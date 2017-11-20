#include "vmm.h"

void instruction_file(){

	// get cwd
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");

	strcat(cwd, "/files/"); // creating fullpath

	// ready to parse each line
	char *fullPath = strcat(cwd,FILENAME);
	FILE *exist; // exist file
	char buff[255];

	exist = fopen(fullPath, "r");
	if (exist){
		while (fgets(buff, 255, (FILE*) exist) != NULL){
  			// printf("%s", buff );
  			tokenize_instruction(buff);
  		}
  		fclose(exist);
	} else {
		printf("File does not exist!!!\n");
	}

}

void tokenize_instruction(char *line){
	char *token;
	char op;
	uint addr;
	int i = 0;

  	token = strtok (line," \n");
  	while (token != NULL){
    	// printf ("%s[%d]: %s\n",KGRN, i, token);

    	// TODO handle two cases: 0x00000000 and non-0x0...
    	if ((token[0] != READ) && (token[0] != WRITE) && (token[0] != PRINT_GENERAL)){

    		// handle 0x00000000
    		if (MAX_STR_LEN == strlen(token)){
    			addr = handle_address(token);
    		} 
    	}

    	else {
    		op = determineOP(token);
    	}

    	i++;
    	token = strtok (NULL, " \n");
  	}

  	
  	if (op == READ || op == WRITE) {
  		printf("entering rw...\n");
  		p_i_rw(op, addr);	
  		instruction_count++;
  	}
  	else if (op == PRINT_FRAME_TABLE || op == PRINT_PAGE_TABLE) {
  		printf("entering pppf...\n");
  		p_i_pppf(op);
  	}
  	
  	printf("\n\n");
  	p_i_pppf('P');
  	p_i_pppf('F');
  	printf("\n-------------\n");
}

uint handle_address(char *addr){

	uint aAddr = (uint) strtol(addr,NULL, 16);

	return aAddr;
}

char determineOP(char *op){

	// is it read op?
	if (op[0] == READ){
		// printf("%c\n", READ);
		return READ;
	}

	// is it write op?
	else if (op[0] == WRITE){
		// printf("%c\n", WRITE);
		return WRITE;
	}

	// is it pp or pf OP?
	else if (op[1] != NULL){
		
		if (op[1] == PRINT_PAGE_TABLE){
			// printf("%c%c\n", PRINT_GENERAL, PRINT_PAGE_TABLE);
			return PRINT_PAGE_TABLE;
		}

		else if (op[1] == PRINT_FRAME_TABLE){
			// printf("%c%c\n", PRINT_GENERAL, PRINT_FRAME_TABLE);
			return PRINT_FRAME_TABLE;
		} 
	}

	// TODO run segfault
	return 'N';
}


