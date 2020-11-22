#include <stdio.h>

void help(){
	printf("RF v0.0.2\n");

	printf("Commands:\n\n");

	printf("-h: help\n");
	printf("Displays program info.\n\n");

	printf("-a: add\n>>rf -a <file path>\n");
	printf("Stages <file path> for processing\n\n");
	
	printf("-l: list\n>>rf -l\n");
	printf("Lists the files staged for processing\n\n");
	
	printf("-r: reset\n>>rf -r\n");
	printf("Resets the list of files staged for processing\n\n");

	

	printf("-w: wrap\n>>rf -w\n");
	printf("Reads the files staged for processing and places them into\none file called output.container\n\n");

	printf("-u: unwrap\n>>rf -u [.container file]\n");
	printf("Takes a .container file and decodes the original files\n");
};

void append(char *path, int argc){
	if(argc < 3){
		printf("path required!\n>>rf -a [path]\n");

	} else {

		FILE *filePointer; //creates a file pointer
		filePointer = fopen("./list.txt", "a"); //opens file @ pointer
	
		if(filePointer == NULL){ //if something went wrong
			printf("an error occured! :(\n");
	
		} else { //otherwise
	
			fprintf(filePointer, "%s\n", path); //write to file
			fclose(filePointer); // close file when finished
			printf("%s added to list.txt successfully!\n", path);
		}
	}
};

void readList(){
	FILE *list = fopen("./list.txt", "rb");
	char filenames[256] = {0};
	
	while(!feof(list)){
		printf(" %s\n", fgets(filenames, 256, list));
	}
	
	printf("\n>>rf -r \nto reset list\n");
}

void resetList(){
	remove("list.txt");
	printf("list.txt removed\n");
}

void wrap(){
	FILE *input = fopen("./list.txt", "rb"); //open list
	FILE *output = fopen("./output.txt", "wb");

	if(input == NULL || output == NULL){
		printf("an error occured :(");
		
	} else {

		char filename[256] = {0};
		int itr = 0;
		//read list file
		while(!feof(input)){ //when file ends
			
			int currentListByte = fgetc(input); //get byte
			
			if(currentListByte == '\n'){ //when current byte is newline
				filename[itr] = '\0'; //add null terminator

				//check if file exists
				FILE *ffile = fopen(filename, "rb");
				if(ffile == NULL){ printf("RF: an error occured :("); break;}
				//write filename + delim to output file
				fprintf(output, "%s@f", filename);
				//read the file byte by byte and prefix any @ symbols with @
				while(!feof(ffile)){
					//get byte
					int in = fgetc(ffile);

					//if byte is an @
					if(in == '@'){
						//output 2 @ symbols to output file
						fputc('@', output);
						fputc('@', output);
					} else if(!feof(ffile)){
						//output in to output file
						fputc(in, output);
					}
				}
				fputc('@', output);
				fputc('e', output);
				fclose(ffile);
				//reset filename string
				itr = 0;
				printf("RF: processed %s\n", filename);
			} else {
				filename[itr] = currentListByte;
				itr++;
			}

		}

	}
	fclose(input);
	fclose(output);
	printf("RF: output.container created\n");
	
}

void unwrap(char *filename){
	FILE *input = fopen(filename, "rb");
	FILE *output;
	int itf = 0;
	char filenam[128];
	int itr = 0;

	///oh my god

	while(!feof(input)){
		if(itf == 0){
			int in = fgetc(input);

			if(in == '@'){
				int in2 = fgetc(input);
				if(in2 == 'f'){
					filenam[itr] = '\0';
					itf = 1;

					output = fopen(filenam, "wb");
					printf("RF: created file %s\n", filenam);
				}
			} else {


				filenam[itr] = in;
				itr++;

			}
		} else {
			//start reading into file listed in $usernam
			int in = fgetc(input);
			if(in == '@'){
				int in2 = fgetc(input);
				
				if(in2 == 'e'){ itr = 0; itf = 0; fclose(output);} else {

					fputc(in, output);
				}


			} else {
				fputc(in, output);
			}
		}
	} //while(!feof(input)){
fclose(input);
} //void unwrap(char *filename){


int main(int argc, char **argv){

	if(argc > 1){//check if args are present

		//parse args
		switch(argv[1][1]){ //get arg
			case 'h': //help
				help();
				break;

			case 'a': //add
				append(argv[2], argc);
				break;
	

			case 'l': //list files
				readList();
				break;


			case 'r': //reset file list
				resetList();
				break;
	

			case 'w': //wrap
				wrap();
				break;


			case 'u': //unwrap
				unwrap(argv[2]);
				break;

			default:
				help();
				break;
		};

	//if no args
	} else {
		help();
	}
	return 0;
};
