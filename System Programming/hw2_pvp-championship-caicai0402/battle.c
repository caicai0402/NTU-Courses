#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "status.h"
#define TIMEOUT 500

typedef struct var{
	char battle_id;
	int pid;
	int ppid;
	int log_file_fd;
	int children_pids[2];
	int pipe_fds[2][2][2];
	
	Status players[2];
}Var;

char exec_table[14][2][9] = {{"./battle", "./battle"}, {"./battle", "./battle"}, {"./battle", "./player"}, {"./battle", "./battle"}, {"./battle", "./battle"}, {"./battle", "./battle"}, {"./player", "./player"}, {"./player", "./player"}, {"./player", "./player"}, {"./player", "./player"}, {"./battle", "./player"}, {"./battle", "./player"}, {"./player", "./player"}, {"./player", "./player"}};
char parent_table[14][2] = {"Z", "A", "A", "B", "B", "C", "D", "D", "E", "E", "F", "F", "K", "L"};
char children_table[14][2][3] = {{"B", "C"}, {"D", "E"}, {"F", "14"}, {"G", "H"}, {"I", "J"}, {"K", "L"}, {"0", "1"}, {"2", "3"}, {"4", "5"}, {"6", "7"}, {"M", "10"}, {"N", "13"}, {"8", "9"}, {"11", "12"}};
Attribute battle_attr[14] = {FIRE, GRASS, WATER, WATER, FIRE, FIRE, FIRE, GRASS, WATER, GRASS, GRASS, GRASS, FIRE, WATER};


int create_log_file(char battle_id, Var *var);
void readPSSM(int fd, Status *status);
void writePSSM(int fd, Status *status);

void init(Var *var);
void waiting(Var *var);
void playing(Var *var);
void passing(Var *var);
void clean(Var *var);

int main (int argc, char *argv[]) {
	// fprintf(stderr, "battle argc: %d, argv: \"%s\", \"%s\", \"%s\"\n", argc, argv[0], argv[1], argv[2]);	

	Var var = {*argv[1], getpid(), atoi(argv[2])};

	// battle’s lifecycle
	init(&var);
	waiting(&var);
	playing(&var);
	if(*argv[1] != 'A')
		passing(&var);
	else
		fprintf(stdout, "Champion is P%d\n", var.players[0].HP > 0? var.players[0].real_player_id : var.players[1].real_player_id);
	clean(&var);
	exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


int create_log_file(char battle_id, Var *var){
	char fileName[16] = "log_battle_.txt";
	fileName[10] = battle_id;
	return open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0777);
}

void init(Var *var){
	var->log_file_fd = create_log_file(var->battle_id, var);

	char pid[10];
	sprintf(pid, "%d", var->pid);

	pipe(var->pipe_fds[0][0]);
	pipe(var->pipe_fds[0][1]);
	if((var->children_pids[0] = fork()) == 0){
		dup2(var->pipe_fds[0][0][0], STDIN_FILENO);
		dup2(var->pipe_fds[0][1][1], STDOUT_FILENO);
		close(var->pipe_fds[0][0][0]);
		close(var->pipe_fds[0][0][1]);
		close(var->pipe_fds[0][1][0]);
		close(var->pipe_fds[0][1][1]);
		char *argv[] = {exec_table[var->battle_id - 'A'][0], children_table[var->battle_id - 'A'][0], pid, NULL};
		if(execv(exec_table[var->battle_id - 'A'][0], argv) < 0){
			perror("erro on exec 1");
			exit(0);
		}
	}

	pipe(var->pipe_fds[1][0]);
	pipe(var->pipe_fds[1][1]);
	if((var->children_pids[1] = fork()) == 0){
		dup2(var->pipe_fds[1][0][0], STDIN_FILENO);
		dup2(var->pipe_fds[1][1][1], STDOUT_FILENO);
		close(var->pipe_fds[1][0][0]);
		close(var->pipe_fds[1][0][1]);
		close(var->pipe_fds[1][1][0]);
		close(var->pipe_fds[1][1][1]);
		char *argv[] = {exec_table[var->battle_id - 'A'][1], children_table[var->battle_id - 'A'][1], pid, NULL};
		if(execv(exec_table[var->battle_id - 'A'][1], argv) < 0){
			perror("erro on exec 2");
			exit(0);
		}
	}

	if(var->children_pids[0] != 0 && var->children_pids[1] != 0){
		close(var->pipe_fds[0][0][0]);
		close(var->pipe_fds[0][1][1]);
		close(var->pipe_fds[1][0][0]);
		close(var->pipe_fds[1][1][1]);
	}
	return ;
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

void waiting(Var *var){
	readPSSM(var->pipe_fds[0][1][0], &var->players[0]);
	dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][0], var->children_pids[0], var->players[0].real_player_id, var->players[0].HP, var->players[0].current_battle_id, var->players[0].battle_ended_flag);
	
	readPSSM(var->pipe_fds[1][1][0], &var->players[1]);
	dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][1], var->children_pids[1], var->players[1].real_player_id, var->players[1].HP, var->players[1].current_battle_id, var->players[1].battle_ended_flag);
	return;
}

void playing(Var *var){
	var->players[0].current_battle_id = var->players[1].current_battle_id = var->battle_id;
	var->players[0].battle_ended_flag = var->players[1].battle_ended_flag = 0;

	do{
		int idx = var->players[0].HP < var->players[1].HP? 0 : 
					var->players[0].HP > var->players[1].HP? 1 :
					var->players[0].real_player_id < var->players[1].real_player_id? 0 : 1;

		var->players[!idx].HP = var->players[!idx].HP - (var->players[idx].attr == battle_attr[var->battle_id - 'A']? 2 * var->players[idx].ATK : var->players[idx].ATK);
		if(var->players[!idx].HP > 0)
			var->players[idx].HP = var->players[idx].HP - (var->players[!idx].attr == battle_attr[var->battle_id - 'A']? 2 * var->players[!idx].ATK : var->players[!idx].ATK);
		
		if(var->players[0].HP <= 0 || var->players[1].HP <= 0)
			var->players[0].battle_ended_flag = var->players[1].battle_ended_flag = 1;
		
		dprintf(var->log_file_fd, "%c,%d pipe to %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][0], var->children_pids[0], var->players[0].real_player_id, var->players[0].HP, var->players[0].current_battle_id, var->players[0].battle_ended_flag);
		writePSSM(var->pipe_fds[0][0][1], &var->players[0]);
		
		dprintf(var->log_file_fd, "%c,%d pipe to %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][1], var->children_pids[1], var->players[1].real_player_id, var->players[1].HP, var->players[1].current_battle_id, var->players[1].battle_ended_flag);
		writePSSM(var->pipe_fds[1][0][1], &var->players[1]);

		if(var->players[0].battle_ended_flag == 0 && var->players[1].battle_ended_flag == 0){
			readPSSM(var->pipe_fds[0][1][0], &var->players[0]);
			dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][0], var->children_pids[0], var->players[0].real_player_id, var->players[0].HP, var->players[0].current_battle_id, var->players[0].battle_ended_flag);

			readPSSM(var->pipe_fds[1][1][0], &var->players[1]);
			dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][1], var->children_pids[1], var->players[1].real_player_id, var->players[1].HP, var->players[1].current_battle_id, var->players[1].battle_ended_flag);
		}
	
	}while(!var->players[0].battle_ended_flag && !var->players[1].battle_ended_flag);

	return ;
}

void passing(Var *var){
	int idx = var->players[0].HP > 0? 0 : 1;

	while(1){
		readPSSM(var->pipe_fds[idx][1][0], &var->players[idx]);
		dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][idx], var->children_pids[idx], var->players[idx].real_player_id, var->players[idx].HP, var->players[idx].current_battle_id, var->players[idx].battle_ended_flag);

		dprintf(var->log_file_fd, "%c,%d pipe to %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, parent_table[var->battle_id - 'A'], var->ppid, var->players[idx].real_player_id, var->players[idx].HP, var->players[idx].current_battle_id, var->players[idx].battle_ended_flag);
		writePSSM(STDOUT_FILENO, &var->players[idx]);

		readPSSM(STDIN_FILENO, &var->players[idx]);
		dprintf(var->log_file_fd, "%c,%d pipe from %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, parent_table[var->battle_id - 'A'], var->ppid, var->players[idx].real_player_id, var->players[idx].HP, var->players[idx].current_battle_id, var->players[idx].battle_ended_flag);

		dprintf(var->log_file_fd, "%c,%d pipe to %s,%d %d,%d,%c,%d\n", var->battle_id, var->pid, children_table[var->battle_id - 'A'][idx], var->children_pids[idx], var->players[idx].real_player_id, var->players[idx].HP, var->players[idx].current_battle_id, var->players[idx].battle_ended_flag);
		writePSSM(var->pipe_fds[idx][0][1], &var->players[idx]);

		if(var->players[idx].battle_ended_flag == 1 && (var->players[idx].current_battle_id == 'A' || var->players[idx].HP <= 0))
			break;
	}

	return ;
}

void clean(Var *var){
	waitpid(var->children_pids[0], NULL, 0);
	waitpid(var->children_pids[1], NULL, 0);
	close(var->log_file_fd);
	close(var->pipe_fds[0][0][0]);
	close(var->pipe_fds[0][0][1]);
	close(var->pipe_fds[0][1][0]);
	close(var->pipe_fds[0][1][1]);
	close(var->pipe_fds[1][0][0]);
	close(var->pipe_fds[1][0][1]);
	close(var->pipe_fds[1][1][0]);
	close(var->pipe_fds[1][1][1]);
	return ;
}
