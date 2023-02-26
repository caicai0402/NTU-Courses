#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "status.h"
#define TIMEOUT 500

typedef struct var{
	int player_id;
	int pid;
	int ppid;
	int log_file_fd;
	Status player;
	Status const_player;
}Var;

int lose_table[14] = {-1, 14, -1, 10, 13, -1, 8, 11, 9, 12, -1, -1, -1, -1};
char parent_table[15][2] = {"G", "G", "H", "H", "I", "I", "J", "J", "M", "M", "K", "N", "N", "L", "C"};

int create_log_file(int player_id);
int create_fifo_file(int player_id, int flags);
void readPSSM(int fd, Status *status);
void writePSSM(int fd, Status *status);
void init(Var *var);
void playing(Var *var);
void closing(Var *var);
void clean(Var *var);

int main (int argc, char *argv[]) {
	// fprintf(stderr, "player argc: %d, argv: \"%s\", \"%s\", \"%s\"\n", argc, argv[0], argv[1], argv[2]);

	Var var = {atoi(argv[1]), getpid(), atoi(argv[2])};
	
	init(&var);
	playing(&var);
	closing(&var);
	clean(&var);
	exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int create_log_file(int player_id){
	char fileName[16] = "log_player_.txt";
	sprintf(fileName, "log_player%d.txt", player_id);
	return open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0777);
}

int create_fifo_file(int player_id, int flags){
	char fifoName[16] = {0};
	sprintf(fifoName, "player%d.fifo", player_id);
	mkfifo(fifoName, 0777);
	return open(fifoName, flags);
}

void readPSSM(int fd, Status *status){
	struct pollfd fdarr[1] = {{fd, POLLIN}};
	while(poll(fdarr, 1, TIMEOUT) == 0);
	read(fd, status, sizeof(Status));
	return ;
}

void writePSSM(int fd, Status *status){
	struct pollfd fdarr[1] = {{fd, POLLOUT}};
	while(poll(fdarr, 1, TIMEOUT) == 0);
	write(fd, status, sizeof(Status));
	return ;
}

void init(Var *var){
	var->player.real_player_id = var->player_id;
	if(0 <= var->player_id && var->player_id <= 7){
		FILE *status_fp = fopen("player_status.txt", "r");
		char attr[6];
		for(int i = 0; i <= var->player_id; i++)
			fscanf(status_fp, "%d %d %s %c %d\n", &var->player.HP, &var->player.ATK, attr, &var->player.current_battle_id, &var->player.battle_ended_flag);
		switch(attr[0]){
			case 'F':
				var->player.attr = FIRE;
				break;
			case 'G':
				var->player.attr = GRASS;
				break;
			case 'W':
				var->player.attr = WATER;
				break;
		}
	}
	else{
		int fifo_fd = create_fifo_file(var->player_id, O_RDONLY);
		readPSSM(fifo_fd, &var->player);
		close(fifo_fd);
	}
	
	var->log_file_fd = create_log_file(var->player.real_player_id);
	if(var->player_id != var->player.real_player_id)
		dprintf(var->log_file_fd, "%d,%d fifo from %d %d,%d\n", var->player_id, var->pid, var->player.real_player_id, var->player.real_player_id, var->player.HP);
	
	var->const_player = var->player;
	return ;
}

void playing(Var *var){
	while(1){
		dprintf(var->log_file_fd, "%d,%d pipe to %s,%d %d,%d,%c,%d\n", var->player_id, var->pid, parent_table[var->player_id], var->ppid, var->player.real_player_id, var->player.HP, var->player.current_battle_id, var->player.battle_ended_flag);
		writePSSM(STDOUT_FILENO, &var->player);

		readPSSM(STDIN_FILENO, &var->player);
		dprintf(var->log_file_fd, "%d,%d pipe from %s,%d %d,%d,%c,%d\n", var->player_id, var->pid, parent_table[var->player_id], var->ppid, var->player.real_player_id, var->player.HP, var->player.current_battle_id, var->player.battle_ended_flag);
		
		if(var->player.battle_ended_flag == 1){
			if(var->player.HP <= 0 || var->player.current_battle_id == 'A')
				break;
			else{
				var->player.HP = var->player.HP <= 0? var->player.HP : var->player.HP + (var->const_player.HP - var->player.HP) / 2;
				var->player.battle_ended_flag = 0;
			}
		}
	}
	return;
}

void closing(Var *var){
	if(var->player_id <= 7 && var->player.current_battle_id != 'A'){
		int fifo_fd = create_fifo_file(lose_table[var->player.current_battle_id - 'A'], O_WRONLY);
		var->player.HP = var->const_player.HP;
		var->player.battle_ended_flag = 0;
		dprintf(var->log_file_fd, "%d,%d fifo to %d %d,%d\n", var->player_id, var->pid, lose_table[var->player.current_battle_id - 'A'], var->player.real_player_id, var->player.HP);
		writePSSM(fifo_fd, &var->player);
		close(fifo_fd);
	}
	return ;
}

void clean(Var *var){
	close(var->log_file_fd);
	return ;
}