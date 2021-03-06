#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "time.h"
#define N 200
#define NREC 1000
#define NSTEP 25

int main(int argc, char *argv[])
{
	int i, j,k,t,rank,size,nper;
	double G = 6.67e-6;
	double m = 1000, r;
	double  *modelxtrue = (double *)malloc(N*N * sizeof(double));
	double  *modelytrue = (double *)malloc(N*N * sizeof(double));
	double  *modelx = (double *)malloc(N*N * sizeof(double));
	double  *modely = (double *)malloc(N*N * sizeof(double));
	double rec[NREC], data[NREC],datatrue[NREC],datarecv[NREC];
	double start, end;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	nper = NREC / size;
	if (rank == 0)start=clock();
	FILE* fp;
	fp = fopen("data.txt","w");
	FILE* fpt;
	fpt = fopen("datatrue.txt", "w");
	//接收器的位置
	for (i = 0; i < NREC; i++)rec[i] = i;
	//产生真实模型的观测值
	for (i = 0; i < NREC; i++)datatrue[i] = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			modelxtrue[i*N + j] = j +200+ 300;
			modelytrue[i*N + j] = -i -200-300;
		}
	}
	for (i = 0; i < NREC; i++) {
		for (j = 0; j < N*N; j++) {
			r = (modelxtrue[j] - rec[i])*(modelxtrue[j] - rec[i]) + modelytrue[j] * modelytrue[j];
			datatrue[i] = datatrue[i] + G * m / r;
		}
	}
	for (i = 0; i < NREC; i++)fprintf(fpt, "%f ", datatrue[i]);
	//
	for (t = 0; t < NSTEP; t++) {
		for (k = 0; k < NSTEP; k++) {
			//初始化
			for (i = 0; i < NREC; i++)data[i] = 0;
			//移动异常体位置
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					modelx[i*N + j] = j + k * 20 + 200;
					modely[i*N + j] = -i - t * 20 - 200;
					//printf("%f %f ", modelx[i*N + j], modely[i*N + j]);
				}
			}
			//计算重力异常值
			for (i = 0; i < nper; i++) {
				for (j = 0; j < N*N; j++) {
					r = (modelx[j] - rec[rank*nper + i])*(modelx[j] - rec[rank*nper + i]) + modely[j] * modely[j];
					data[rank*nper + i] = data[rank*nper + i] + G * m / r;
				}
			}
			//输出重力异常值
			MPI_Gather(data+rank*nper, nper, MPI_DOUBLE, datarecv, nper, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			if (rank == 0) {
				for (i = 0; i < NREC; i++)fprintf(fp, "%f ", datarecv[i]);
				fprintf(fp, "\n");
			}
		}
	}
	if (rank == 0) {
		end = clock();
		printf("time cost is: %f", end - start);
	}
	MPI_Finalize();
    return 0;
}

