#include <complex>
#include <iostream>
#include <omp.h>
#include <sys/time.h>

using namespace std;

long wtime(){
   	struct timeval t;
   	gettimeofday(&t, NULL);
   	
	return t.tv_sec*1000000 + t.tv_usec;
}

int main(int argc, char** argv){
	int max_row, max_column, max_n, chunk;
	long start_time, end_time;
	
	max_row = atoi(argv[1]);
	max_column = atoi(argv[2]);
	max_n = atoi(argv[3]);

	start_time = wtime();
	
	char **mat = (char**)malloc(sizeof(char*)*max_row);
	
	chunk = max_row/5;
	
	#pragma omp for schedule(static, chunk)
	for (int i=0; i<chunk;i++)
		mat[i]=(char*)malloc(sizeof(char)*max_column);
	
	#pragma omp for schedule(static, chunk)
	for(int r = 0; r < chunk; ++r){
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}
	
	#pragma omp for schedule(static, chunk)
	for(int r = 0; r < chunk; ++r){
		for(int c = 0; c < max_column; ++c)
			std::cout << mat[r][c];
		cout << '\n';
	}	
	
	end_time = wtime();	

	printf("%ld usec\n", (long) (end_time - start_time));
}
