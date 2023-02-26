#include "header.h"

#define MIN_SORT_SIZE 32
#ifdef PROCESS
#define PNUM 1
#define LAYER 7
#elif defined THREAD
#define PNUM 7
#define LAYER 5
#endif

movie_profile* movie_profiles[MAX_MOVIES];
unsigned int num_of_movies = 0;

// // Global request queue and pointer to front of queue
// // TODO: critical section to protect the global resources
request* reqs[MAX_REQ];
unsigned int num_of_reqs = 0;
int front = -1;

// /* Note that the maximum number of processes per workstation user is 512. * 
//  * We recommend that using about <256 threads is enough in this homework. */

// //mutex
pthread_mutex_t lock; 

void initialize(FILE* fp);
request* read_request();
int pop();

// CaiCai
typedef struct arg{
	char **movies;
	double *pts;
	int *idxs;
	int size;
	int layer;
}Arg;

void multireq(int pnum, int layer);
void *solver(void *layer);
void argcCheck(int argc);
void readData();
void init(char ***movies, double **pts, int **idxs);
int filter(request *req, char **movies, double *pts);
void *thr2mergeSort(void *arg);
void mergeSort(char **movies, double *pts, int *idxs, int size, int layer);
void output(int id, char **movies, int *idxs, int size);
void release(char **movies, double *pts, int *idxs);

int main(int argc, char *argv[]){
	argcCheck(argc);
	readData();
	multireq(PNUM, LAYER);
	return 0;
}

void argcCheck(int argc){
	if(argc != 1){
#ifdef PROCESS
		fprintf(stderr,"usage: ./pserver\n");
#elif defined THREAD
		fprintf(stderr,"usage: ./tserver\n");
#endif
		exit(-1);
	}
	return ;
}

void readData(){
	FILE *fp;
	if ((fp = fopen("./data/movies.txt","r")) == NULL)
		ERR_EXIT("fopen");
	initialize(fp);
	assert(fp != NULL);
	fclose(fp);
	return ;
}

void multireq(int pnum, int layer){
	pthread_mutex_init(&lock, 0);
	pthread_t pids[pnum];
	for(int i = 0; i < pnum; i++)
		pthread_create(&pids[i], NULL, solver, (void *)(&layer));
	for(int i = 0; i < pnum; i++)
		pthread_join(pids[i], NULL);
	pthread_mutex_destroy(&lock);
}

void *solver(void *layer){
	char **movies = NULL;
	double *pts = NULL;
	int *idxs = NULL;

	while(1){
		pthread_mutex_lock(&lock);
		int idx = pop();
		pthread_mutex_unlock(&lock);
		if(idx >= num_of_reqs || reqs[idx] == NULL)
			break;
		else if(movies == NULL && pts == NULL && idxs == NULL)
			init(&movies, &pts, &idxs);

		int size = filter(reqs[idx], movies, pts);
		mergeSort(movies, pts, idxs, size, *((int*)layer));
		output(reqs[idx]->id, movies, idxs, size);
	}

	if(movies != NULL && pts != NULL && idxs != NULL)
		release(movies, pts, idxs);
	pthread_exit(NULL);
	return 0;
}

void init(char ***movies, double **pts, int **idxs){

#ifdef PROCESS
	*movies = (char **)mmap(NULL, sizeof(char *) * MAX_MOVIES, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	(*movies)[0] = (char *)mmap(NULL, sizeof(char) * MAX_MOVIES * MAX_LEN, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for(int i = 1; i < MAX_MOVIES; i++)
		(*movies)[i] = (*movies)[i-1] + MAX_LEN;
	*pts = (double *)mmap(NULL, sizeof(double) * MAX_MOVIES, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*idxs = (int *)mmap(NULL, sizeof(int) * MAX_MOVIES, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
#elif defined THREAD
	*movies = (char **)malloc(sizeof(char *) * MAX_MOVIES);
	(*movies)[0] = (char *)malloc(sizeof(char) * MAX_MOVIES * MAX_LEN);
	for(int i = 1; i < MAX_MOVIES; i++)
		(*movies)[i] = (*movies)[i-1] + MAX_LEN;
	*pts = (double *)malloc(sizeof(double) * MAX_MOVIES);
	*idxs = (int *)malloc(sizeof(int) * MAX_MOVIES);
	
#endif

	return ;
}

int filter(request *req, char *movies[MAX_LEN], double *pts){
	int size = 0;
	for(int i = 0; i < num_of_movies; i++){
		if((req->keywords[0] == '*' && req->keywords[1] == '\0') || strstr(movie_profiles[i]->title, req->keywords) != NULL){
			strcpy(movies[size], movie_profiles[i]->title);
			pts[size] = 0;
			for(int j = 0; j < NUM_OF_GENRE; j++)
				pts[size] += movie_profiles[i]->profile[j] * req->profile[j];
			size ++;
		}
	}
	return size;
}

void *thr2mergeSort(void *arg){
	Arg *tmp = (Arg *)arg;
	mergeSort(tmp->movies, tmp->pts, tmp->idxs, tmp->size, tmp->layer);
	pthread_exit(NULL);
	return 0;
}

void mergeSort(char **movies, double *pts, int *idxs, int size, int layer){
	if(size <= MIN_SORT_SIZE || layer == 0){
		char *tmp[size];
		for(int i = 0; i < size; i++)
			tmp[i] = movies[i];
		sort(tmp, pts, size);
		for(int i = 0; i < size; i++){
			strcpy(movies[i], tmp[i]);
			idxs[i] = i;
			free(tmp[i]);
		}
	}
	else{
		int mid = size / 2, tmp[size];
		
#ifdef PROCESS
		pid_t pid;
		if((pid = fork()) == 0){ // child
			mergeSort(movies, pts, idxs, mid, layer-1);
			exit(0);
		}
		else{ // parrent
			mergeSort(&movies[mid], &pts[mid], &idxs[mid], size - mid, layer-1);
			waitpid(pid, NULL, 0);
		}

#elif defined THREAD
		pthread_t tid;
		Arg arg = {movies, pts, idxs, mid, layer-1};
		pthread_create(&tid, NULL, thr2mergeSort, (void *)&arg);
		mergeSort(&movies[mid], &pts[mid], &idxs[mid], size - mid, layer-1);
		pthread_join(tid, NULL);

#endif
		for(int i = 0, j = mid, idx = 0; i < mid || j < size; ){
			if(i == mid)
				tmp[idx ++] = mid + idxs[j++];
			else if(j == size)
				tmp[idx ++] = idxs[i++];
			else if(pts[idxs[i]] > pts[mid + idxs[j]])
				tmp[idx ++] = idxs[i++];
			else if(pts[idxs[i]] < pts[mid + idxs[j]])
				tmp[idx ++] = mid + idxs[j++];
			else if(strcmp(movies[idxs[i]], movies[mid + idxs[j]]) < 0)
				tmp[idx ++] = idxs[i++];
			else
				tmp[idx ++] = mid + idxs[j++];
		}
		for(int i = 0; i < size; i++)
			idxs[i] = tmp[i];
	}
	return ;
}

void output(int id, char **movies, int *idxs, int size){
	char fileName[12];
#ifdef PROCESS
	sprintf(fileName, "%dp.out", id);
#elif defined THREAD
	sprintf(fileName, "%dt.out", id);
#endif
	FILE *fp = fopen(fileName, "w");
	for(int i = 0; i < size; i++)
		fprintf(fp, "%s\n", movies[idxs[i]]);
	fclose(fp);
	return ;
}

void release(char **movies, double *pts, int *idxs){

#ifdef PROCESS
	munmap(movies[0], sizeof(char) * MAX_MOVIES * MAX_LEN);
	munmap(movies, sizeof(char *) * MAX_MOVIES);
	munmap(pts, sizeof(double) * MAX_MOVIES);
	munmap(idxs, sizeof(int) * MAX_MOVIES);

#elif defined THREAD
	free(movies[0]);
	free(movies);
	free(pts);
	free(idxs);

#endif
	
	return ;
}

/**=======================================
 * You don't need to modify following code *
 * But feel free if needed.                *
 =========================================**/

request* read_request(){
	int id;
	char buf1[MAX_LEN],buf2[MAX_LEN];
	char delim[2] = ",";

	char *keywords;
	char *token, *ref_pts;
	char *ptr;
	double ret,sum;

	scanf("%u %254s %254s",&id,buf1,buf2);
	keywords = malloc(sizeof(char)*strlen(buf1)+1);
	if(keywords == NULL){
		ERR_EXIT("malloc 1");
	}

	memcpy(keywords, buf1, strlen(buf1));
	keywords[strlen(buf1)] = '\0';

	double* profile = malloc(sizeof(double)*NUM_OF_GENRE);
	if(profile == NULL){
		ERR_EXIT("malloc 2");
	}
	sum = 0;
	ref_pts = strtok(buf2,delim);
	for(int i = 0;i < NUM_OF_GENRE;i++){
		ret = strtod(ref_pts, &ptr);
		profile[i] = ret;
		sum += ret*ret;
		ref_pts = strtok(NULL,delim);
	}

	// normalize
	sum = sqrt(sum);
	for(int i = 0;i < NUM_OF_GENRE; i++){
		if(sum == 0)
				profile[i] = 0;
		else
				profile[i] /= sum;
	}

	request* r = malloc(sizeof(request));
	if(r == NULL){
		ERR_EXIT("malloc 3");
	}

	r->id = id;
	r->keywords = keywords;
	r->profile = profile;

	return r;
}

/*=================initialize the dataset=================*/
void initialize(FILE* fp){

	char chunk[MAX_LEN] = {0};
	char *token,*ptr;
	double ret,sum;
	int cnt = 0;

	assert(fp != NULL);

	// first row
	if(fgets(chunk,sizeof(chunk),fp) == NULL){
		ERR_EXIT("fgets");
	}

	memset(movie_profiles,0,sizeof(movie_profile*)*MAX_MOVIES);	

	while(fgets(chunk,sizeof(chunk),fp) != NULL){
		
		assert(cnt < MAX_MOVIES);
		chunk[MAX_LEN-1] = '\0';

		const char delim1[2] = " "; 
		const char delim2[2] = "{";
		const char delim3[2] = ",";
		unsigned int movieId;
		movieId = atoi(strtok(chunk,delim1));

		// title
		token = strtok(NULL,delim2);
		char* title = malloc(sizeof(char)*strlen(token)+1);
		if(title == NULL){
			ERR_EXIT("malloc 4");
		}
		
		// title.strip()
		memcpy(title, token, strlen(token)-1);
	 	title[strlen(token)-1] = '\0';

		// genres
		double* profile = malloc(sizeof(double)*NUM_OF_GENRE);
		if(profile == NULL){
			ERR_EXIT("malloc 5");
		}

		sum = 0;
		token = strtok(NULL,delim3);
		for(int i = 0; i < NUM_OF_GENRE; i++){
			ret = strtod(token, &ptr);
			profile[i] = ret;
			sum += ret*ret;
			token = strtok(NULL,delim3);
		}

		// normalize
		sum = sqrt(sum);
		for(int i = 0; i < NUM_OF_GENRE; i++){
			if(sum == 0)
				profile[i] = 0;
			else
				profile[i] /= sum;
		}

		movie_profile* m = malloc(sizeof(movie_profile));
		if(m == NULL){
			ERR_EXIT("malloc 6");
		}

		m->movieId = movieId;
		m->title = title;
		m->profile = profile;

		movie_profiles[cnt++] = m;
	}
	num_of_movies = cnt;

	// request
	scanf("%d",&num_of_reqs);
	assert(num_of_reqs <= MAX_REQ);
	for(int i = 0; i < num_of_reqs; i++){
		reqs[i] = read_request();
	}
}

int pop(){
	front+=1;
	return front;
}
/*========================================================*/