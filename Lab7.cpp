#include <iostream>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
void displayhelp();
void displaymenu();
class Signal{
	private:
		int length;
		double max;
		double average;
		double *data;
	public:
		Signal();
		Signal(int filenum);
		Signal(const char* filename);
		~Signal();
		void readFile(int filenum);
		void offset(double num);
		void scale(double n);
		void stats(char* str);
		void center();
		void normal();
		void sig_info();//new
		void save_File(char* str);//new
};
Signal::Signal(){
	length = 0;
	max = 0;
	average =0;
	data = NULL;
}
Signal::Signal(int filenum){
	//open file
	//cout << "Int constructor" << endl;
	char str[16];
	sprintf(str, "Raw_data_%02d.txt", filenum);
	//cout << "filename: " << str << endl;
	FILE* fp;
	fp = fopen(str, "r");//open file
	if(fp==NULL){
		cout << "File not loaded correctly" << endl;
		return;
	}
	
	fscanf(fp, "%d %lf", &length, &max);//get size and max value
	//cout << "Length: " << length << endl << "Max: " << max << endl;
	data = new double[length];
	for(int i=0; i<length; i++){
			fscanf(fp,"%lf", &data[i]);//get data from file
	}
	cout << "Read file correctly" << endl;
    fclose(fp);//close file
}
Signal::Signal(const char* filename){
	cout << "filename: " << filename << endl;
	FILE* fp;
	fp = fopen(filename, "r");//open file
	if(fp==NULL){
		cout << "File not loaded correctly" << endl;
		return;
	}
	
	fscanf(fp, "%d %lf", &length, &max);//get size and max value
	cout << "Length: " << length << endl << "Max: " << max << endl;
	data = new double[length];
	for(int i=2; i<data[0]+2; i++){
			fscanf(fp,"%lf", &data[i]);//get data from file
	}
	cout << "Read file correctly" << endl;
    	fclose(fp);//close file
}
Signal::~Signal(){
	delete data;
}
void Signal::readFile(int filenum){
	char str[16];
	sprintf(str, "Raw_data_%02d.txt", filenum);
	//cout << "filename: " << str << endl;
	FILE* fp;
	fp = fopen(str, "r");//open file
	if(fp==NULL){
		cout << "File not loaded correctly" << endl;
		return;
	}
	
	fscanf(fp, "%d %lf", &length, &max);//get size and max value
	//cout << "Length: " << length << endl << "Max: " << max << endl;
	data = new double[length];
	for(int i=0; i<length; i++){
			fscanf(fp,"%lf", &data[i]);//get data from file
	}
	//cout << "Read file correctly" << endl;
    fclose(fp);//close file

}
void Signal::save_File(char* str){
	FILE* fp = fopen(str, "w");//open file
	if(fp==NULL)
			printf("error opening file\n");
	double newmax=this->data[0];
	for(int i=0; i<this->length; i++){
		if(this->data[i]>newmax)
			newmax = this->data[i];
	}
	this->max = newmax;
	fprintf(fp,"%d %.2lf\n", length, max);//print first row

	int i;
	for(i=0; i<(length); i++){
		fprintf(fp, "%.2lf\n", data[i]);//print data to file
	}
	fclose(fp);//close file
	printf("Data printed to %s\n", str);
}
void Signal::offset(double num){
	int i;
    for(i=0; i<length; i++){
        data[i]+=num;
    }
}
void Signal::scale(double n){
    int i;
    printf("scale: %.2lf\n", n);
    for(i=0; i<length; i++){
        data[i]*=n;
    }	
}
void Signal::stats(char* str){
    double total=0, average;

    int i;
    for(i=0; i<length; i++){
        total += data[i];//get total for average
        if(data[i]>max)
            max = data[i];//reset max if larger number is found
    }
    average = total/length;
    FILE* fp = fopen(str, "w");//write info to file
    fprintf(fp, "%.4lf %.4lf\n", average, max);
    printf("Data printed to %s\n\n", str);
    fclose(fp);//close file	
}
void Signal::center(){
    double total=0, average;
    int i;
    //printf("Data[2]: %.2lf\n", data[2]);
    for(i=0; i<length; i++){
        total += data[i];//get total for average
        //printf("\nTotal: %lf\n", total);
    }
    average = total/length;
   // printf("\naverage: %lf\n\n", average);
    offset((average*-1));//offset data with average to center	
}
void Signal::normal(){
    int i;
    for(i=0; i<length; i++){
        if(data[i]>max)
            max = data[i];//reset max if larger number is found
    }
    scale((1/max));//call scale function with scale of 1/max
	
}

void Signal::sig_info(){
	cout << "Current max: " << max << endl << "Current average: ";
	double total=0;
	for(int i=0; i<length; i++){
		total+=data[i];
	}
	average = total/length;
	cout << average << endl;
}//new
int main(int argc, char**argv){
	//command line handling
	char str[15], newstr[20], fstr[20];
	
	int i, filenum=0, s=0, c=0, n=0, r=0, f=0, q=1;
	
	double offset_num=0, scale_num=0;
	if(argc == 1){
		while(q!=0){
			cout << "Enter a file number (1-10) or 0 to quit" << endl << ">>";
			cin >> filenum;
		while(filenum < 0 || filenum >10){
			cout << "Enter a file number (1-11) or 0 to quit" << endl << ">>";
			cin >> filenum;
		}
			if(filenum==0){
				cout << "Quitting" << endl;
				return 0;
			}
			char *argstr = new char[50];
			char *arg = new char[20];
			Signal sig(filenum);
			displaymenu();
			cout << endl << "Enter command line: " << endl;
			cin.ignore(1000, '\n');
			cin.get(argstr, 50);			
			
			//cout << "Command line: " << argstr << endl;
			arg = strtok(argstr, " ");
			while(arg!=NULL){
				//printf("Argument: %s\n", arg);
				
					if(arg[0] == '-'){
						//printf("in loop\n");
							switch(arg[1]){
							/*case 'n':
									arg = strtok(NULL, " ");
									if(arg==NULL || (filenum = atoi(arg))==0){
											printf("Invalid format. Number must follow -n\n");
											return 0;//if no num follows -n
									}
									printf("File chosen was: %d\n", filenum);
									break;*/
							case 'o':
									arg = strtok(NULL, " ");
									if(arg==NULL || (offset_num = strtod(arg, NULL))==0){
											printf("Invalid format. Number>0 must follow -o\n");
											printf("Ignoring -o command\n");
											offset_num = 0;//if no num follows
											break;
									}
									//printf("Offset value is: %lf\n", offset_num);
									break;
							case 's':
									arg = strtok(NULL, " ");
									if(arg==NULL|| (scale_num = strtod(arg, NULL))==0){//if no num follows  
										printf("Invalid format. Number>0 must follow -s\n");
										printf("Ignoring -s command\n");
										scale_num=0;
										break;
									}
									printf("Scale num is: %.2lf\n", scale_num);
									//printf("Scale value is: %lf\n", scale_num);
									break;

							case 'S':
									s=1;//statistic command was selected
									break;
							case 'C':
									c=1;//center command was selected
									break;
							case 'N':
									n=1;//normal data command was selected
									break;
							case 'r':
									r=1;//copy file command was selected
									arg = strtok(NULL, " ");
									if(arg==NULL || arg[0] == '-'){//name did not follow
										printf("Invalid format. File name must follow -r\n");
										r=0;//if no name follows ignore
										printf("Ignoring -r command\n");
										break;
									}
									sprintf(newstr, "%s", arg);
									printf("New file: %s\n", newstr);
									break;
							/*case 'f':
									f=1;//copy file command was selected
									arg = strtok(NULL, " ");
									if(arg==NULL || arg[0] == '-'){//name did not follow
										printf("Invalid format. File name must follow -f\n");
										return 0;
									}
									sprintf(fstr, "%s", arg);
									printf("File: %s\n", fstr);
									break;*/
							case 'h'://help command was selected
									displayhelp();
									return 0;
									break;
							default:
									printf("WARNING: argument '%s' was not recognized\n", arg);
									break;//display warnings
							}
					}
					else
						printf("WARNING: argument %s was not recognized\n", arg);
					arg = strtok(NULL, " ");
			}
			sig.readFile(filenum); //temp holds data so it wont be lost
			if(scale_num != 0){//if scale was on command line
				sig.scale(scale_num);
				
				if(r==1)
					sprintf(str, "%s_Scaled_data_%02d.txt", newstr, filenum);
				else
					sprintf(str, "Scaled_data_%02d.txt", filenum);
				sig.save_File(str);
				sig.sig_info();
			}
			sig.readFile(filenum);//reset data
			if(offset_num != 0){//if offset was on command line
				sig.offset(offset_num);
				if(r==1)
					sprintf(str, "%s_Offset_data_%02d.txt", newstr, filenum);
				else
					sprintf(str, "Offset_data_%02d.txt", filenum);
				sig.save_File(str);
				sig.sig_info();
			}
			sig.readFile(filenum); //reset data
			if(s == 1){//if statistics was on command line
				if(r==1)
					sprintf(str, "%s_Statistics_%02d.txt", newstr, filenum);
				else
					sprintf(str, "Statistics_%02d.txt", filenum);
				sig.stats(str);
				sig.sig_info();
			}
			sig.readFile(filenum); //reset data
			if(n == 1){//if normalize data was on command line
				if(r==1)
					sprintf(str, "%s_Normalized_data_%02d.txt", newstr, filenum);
				else
					sprintf(str, "Normalized_data_%02d.txt", filenum);
				sig.normal();
				sig.save_File(str);
				sig.sig_info();
			}
			sig.readFile(filenum); //reset data
			if(c == 1){//if center data was on command line
			   sprintf(str, "Raw_data_%02d.txt", filenum);
				//printf("File chosen: %s\n", str);
				//readFile(data, str);//read file
				 if(r==1)
					sprintf(str, "%s_Centered_data_%02d.txt", newstr, filenum);
				else
					sprintf(str, "Centered_data_%02d.txt", filenum);
					
				sig.center();
				sig.save_File(str);
				sig.sig_info();
			}
			
		}
		//q=0;
	}
	
	else{
		for(i=1; i<argc; i++){
				if(argv[i][0] == '-'){
						switch(argv[i][1]){
						case 'n':
								i++;//increment i by one, should be a num
								if(i>=argc || (filenum = atoi(argv[i]))==0){
										printf("Invalid format. Number must follow -n\n");
										return 0;//if no num follows -n
								}
								//printf("File chosen was: %d\n", filenum);
								break;
						case 'o':
								i++;//increment i by one, should be a num
								if(i>=argc || (offset_num = strtod(argv[i], NULL))==0){
										printf("Invalid format. Number>0 must follow -o\n");
										printf("Ignoring -o command\n");
										offset_num = 0;//if no num follows
										break;
								}
								//printf("Offset value is: %lf\n", offset_num);
								break;
						case 's':
								i++;//increment i by one, should be a num
								if(i>=argc|| (scale_num = strtod(argv[i], NULL))==0){//if no num follows  
									printf("Invalid format. Number>0 must follow -s\n");
									printf("Ignoring -s command\n");
									scale_num=0;
									break;
								}
								printf("Scale num is: %.2lf\n", scale_num);
								//printf("Scale value is: %lf\n", scale_num);
								break;

						case 'S':
								s=1;//statistic command was selected
								break;
						case 'C':
								c=1;//center command was selected
								break;
						case 'N':
								n=1;//normal data command was selected
								break;
						case 'f':
								f=1;//copy file command was selected
								i++;
								if(i>=argc || argv[i][0] == '-'){//name did not follow
									printf("Invalid format. File name must follow -f\n");
									return 0;
								}
								sprintf(fstr, "%s", argv[i]);
								
								break;
						case 'r':
								r=1;//copy file command was selected
								i++;
								if(i>=argc || argv[i][0] == '-'){//name did not follow
									printf("Invalid format. File name must follow -r\n");
									r=0;//if no name follows ignore
									printf("Ignoring -r command\n");
									break;
								}
								sprintf(newstr, "%s", argv[i]);
								printf("New file: %s\n", newstr);
								break;
						case 'h'://help command was selected
								displayhelp();
								return 0;
								break;
						default:
								printf("WARNING: argument '%s' was not recognized\n", argv[i]);
								break;//display warnings
						}
				}
				else
					printf("WARNING: argument '%s' was not recognized\n", argv[i]);
				
		}
		
		Signal sig1, temp;
		if(f==1){
			Signal sig2(fstr);
			sig1 = sig2;
		}
		else{
			if(filenum==0){
				printf("Invalid arguments. No file was chosen\n");
				printf("Type './My_Lab5_program -h' for more info\n");
				return 0;
			}
			Signal sig2(filenum);
			sig1 = sig2;
		}
		temp = sig1; //temp holds data so it wont be lost
		if(scale_num != 0){//if scale was on command line
			sig1.scale(scale_num);
			
			if(r==1)
				sprintf(str, "%s_Scaled_data_%02d.txt", newstr, filenum);
			else
				sprintf(str, "Scaled_data_%02d.txt", filenum);
			sig1.save_File(str);
			sig1.sig_info();
		}
		sig1 = temp;//reset data
		if(offset_num != 0){//if offset was on command line
			sig1.offset(offset_num);
			if(r==1)
				sprintf(str, "%s_Offset_data_%02d.txt", newstr, filenum);
			else
				sprintf(str, "Offset_data_%02d.txt", filenum);
			sig1.save_File(str);
			sig1.sig_info();
		}
		sig1=temp; //reset data
		if(s == 1){//if statistics was on command line
			if(r==1)
				sprintf(str, "%s_Statistics_%02d.txt", newstr, filenum);
			else
				sprintf(str, "Statistics_%02d.txt", filenum);
			sig1.stats(str);
			sig1.sig_info();
		}
		sig1=temp; //reset data
		if(n == 1){//if normalize data was on command line
			if(r==1)
				sprintf(str, "%s_Normalized_data_%02d.txt", newstr, filenum);
			else
				sprintf(str, "Normalized_data_%02d.txt", filenum);
			sig1.normal();
			sig1.save_File(str);
			sig1.sig_info();
		}
		sig1=temp; //reset data
		if(c == 1){//if center data was on command line
		   sprintf(str, "Raw_data_%02d.txt", filenum);
			//printf("File chosen: %s\n", str);
			//readFile(data, str);//read file
			 if(r==1)
				sprintf(str, "%s_Centered_data_%02d.txt", newstr, filenum);
			else
				sprintf(str, "Centered_data_%02d.txt", filenum);
				
			sig1.center();
			sig1.save_File(str);
			sig1.sig_info();
		}
		
	}
	
	return 0;
}
void displayhelp(){

	cout <<"Options:" << endl <<"-n: File number (value needed)" << endl << "-o: offset value (value needed)"
		<<endl <<"-s: scale factor (value needed)\n-S: get statistics\n-C: center the signal\n"
		<<"-N: normalize signal\n-r: Rename files (name needed)\n-h: help\n\n";
}
void displaymenu(){

	cout <<"Options:" << endl << "-o: offset value (value needed)"
		<<endl <<"-s: scale factor (value needed)\n-S: get statistics\n-C: center the signal\n"
		<<"-N: normalize signal\n-r: Rename files (name needed)\n";
}