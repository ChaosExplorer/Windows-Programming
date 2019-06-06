#include<limits>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <cuda.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include "cuda_pairhmm.h"

#define BSIZE 128
#define BALIGN(a) (((a)+127)/128*128)

using namespace std;
using namespace cuda_pairhmm;

struct NUM_ADD
{
    short2 read_haplotype_number;
    int address_array;
};

__global__ void  pairHMM( int size, char * data,  NUM_ADD * num_add, float * result,float * MG,float * DG, float * IG)
{
    int offset=blockIdx.x;

    __shared__ short2 read_haplotype_number;
    __shared__ char * read_base_array;
    __shared__ float * parameter_array;
    __shared__ char4 * haplotype_base_array;
    __shared__ float * MGG;
    __shared__ float * IGG;
    __shared__ float * DGG;
    if(threadIdx.x==0)
    {
        MGG=MG+offset*500;
        DGG=IG+offset*500;
        IGG=DG+offset*500;
    }

    while(offset<size)
    {
        float result_block=0;
        __shared__ int round;
        __shared__ int skip;

        //as each time it will deal with 2 read&haplotype pairs
        // each block deal with one pairs of haplotype & read

        if( threadIdx.x==0)
        {
            read_haplotype_number=num_add[offset].read_haplotype_number;
            read_base_array=(char *) (data+num_add[offset].address_array);
            parameter_array=(float *) (read_base_array+(read_haplotype_number.x+BSIZE-1)/BSIZE*BSIZE);
            skip=(sizeof(float)*read_haplotype_number.x+BSIZE-1)/BSIZE*BSIZE/sizeof(float);
            haplotype_base_array=(char4 *) (parameter_array+skip*4);
            round=(read_haplotype_number.x+blockDim.x-1)/blockDim.x;
        }
        __syncthreads();

        __shared__ char haplotype_base_in_char[500];
        int hh=(read_haplotype_number.y+4-1)/4;
        int tt=(hh+blockDim.x-1)/blockDim.x;
        for(int ii=0;ii<tt;ii++)
        {
            int aa=threadIdx.x+ii*blockDim.x;
            if(aa< hh)
            {
                char4 haplotype_base_in_thread;
                haplotype_base_in_thread=haplotype_base_array[aa];
                haplotype_base_in_char[aa*4]=haplotype_base_in_thread.x;
                haplotype_base_in_char[aa*4+1]=haplotype_base_in_thread.y;
                haplotype_base_in_char[aa*4+2]=haplotype_base_in_thread.z;
                haplotype_base_in_char[aa*4+3]=haplotype_base_in_thread.w;
            }
        }
        __syncthreads();

        __shared__ float MM[130]; //left
        __shared__ float DD[130]; //left
        __shared__ float II[130]; //left
        __shared__ float MM2[130];
        __shared__ float DD2[130];
        __shared__ float II2[130];
        char read_base;
        float D_0=1.329228e+36/(float)read_haplotype_number.y;
        if(threadIdx.x==0)
        {
            MM[0]=0;
            DD[0]=D_0;
            II[0]=0;
            MM2[0]=0;
            II2[0]=0;
            DD2[0]=D_0;
        }
        int read_number=read_haplotype_number.x;
        int round_size;
        for(int i=0;i<round;i++)
        {
            round_size=(read_number>blockDim.x)?blockDim.x:read_number;
            read_number=(read_number>blockDim.x)?read_number-blockDim.x:0;
            char read_base;
            float M=1.0f;
            float Qm,Qm_1,alpha,beta,delta,epsion,xiksi;
            if(threadIdx.x<round_size ) // tid is from 0 ~ round_size-1
            {
                read_base=read_base_array[threadIdx.x+blockDim.x*i];
                delta=parameter_array[threadIdx.x+blockDim.x*i+skip];
                xiksi=parameter_array[threadIdx.x+blockDim.x*i+2*skip];
                alpha=parameter_array[threadIdx.x+blockDim.x*i+3*skip];
                epsion=0.1;
                beta=M-epsion;
                Qm=parameter_array[threadIdx.x+blockDim.x*i];
                Qm_1=M-Qm;
                Qm=fdividef(Qm,3.0f);
            }

            M=0;
            float I=0; //now
            float D=0; //now

            float MMID=0;
            float MMID2=0;
            float M2=0;
            float I2=0;
            float D2=0;

            int current_haplotype_id=0;
            for(int j=0;j<round_size+(read_haplotype_number.y+1)/2-1;j++)
            {
                int aa=j-threadIdx.x;
                if( aa>=0 && (current_haplotype_id<read_haplotype_number.y))
                {
                    if(i>0&&threadIdx.x==0)
                    {
                        MM[0]=MGG[current_haplotype_id];
                        II[0]=IGG[current_haplotype_id];
                        DD[0]=DGG[current_haplotype_id];
                        MM2[0]=MGG[current_haplotype_id+1];
                        II2[0]=IGG[current_haplotype_id+1];
                        DD2[0]=DGG[current_haplotype_id+1];
                    }

                    if(threadIdx.x==0 && i==0)
                        MMID2=__fmul_rn(beta,D_0);

                    float MID=__fadd_rn(II[threadIdx.x],DD[threadIdx.x]);
                    float DDM=__fmul_rn(M2,xiksi);
                    float IIMI=__fmul_rn(II[threadIdx.x],epsion);
                    float MIIDD=__fmul_rn(beta,MID);
                    char haplotype_base_each=haplotype_base_in_char[current_haplotype_id];
                    float aa=(haplotype_base_each==read_base)? Qm_1:Qm;

                    M=__fmul_rn(aa,MMID2);
                    D=__fmaf_rn(D2,epsion,DDM);
                    I=__fmaf_rn(MM[threadIdx.x],delta,IIMI);
                    MMID=__fmaf_rn(alpha,MM[threadIdx.x],MIIDD);

                    current_haplotype_id++;

                    if(threadIdx.x==round_size-1 && i==round-1)
                        result_block=__fadd_rn(result_block,__fadd_rn(M,I));

                    if(current_haplotype_id<read_haplotype_number.y)
                    {
                        MID=__fadd_rn(II2[threadIdx.x],DD2[threadIdx.x]);
                        DDM=__fmul_rn(M,xiksi);
                        IIMI=__fmul_rn(II2[threadIdx.x],epsion);

                        haplotype_base_each=haplotype_base_in_char[current_haplotype_id];
                        aa=(haplotype_base_each==read_base)? Qm_1:Qm;

                        MIIDD=__fmul_rn(beta,MID);
                        M2=__fmul_rn(aa,MMID);
                        I2=__fmaf_rn(MM2[threadIdx.x],delta, IIMI);
                        D2=__fmaf_rn(D,epsion,DDM);
                        MMID2=__fmaf_rn(alpha, MM2[threadIdx.x], MIIDD);

                        current_haplotype_id++;
                        if(threadIdx.x==round_size-1&& i==round-1)
                            result_block=__fadd_rn(result_block,__fadd_rn(M2,I2));
                    }

                }
                __syncthreads();

                MM[threadIdx.x+1]=M;
                DD[threadIdx.x+1]=D;
                II[threadIdx.x+1]=I;

                MM2[threadIdx.x+1]=M2;
                DD2[threadIdx.x+1]=D2;
                II2[threadIdx.x+1]=I2;
                if(i<round-1&&threadIdx.x==round_size-1 )
                {
                    MGG[current_haplotype_id-1]=M2;
                    IGG[current_haplotype_id-1]=I2;
                    DGG[current_haplotype_id-1]=D2;

                    MGG[current_haplotype_id-2]=M;
                    IGG[current_haplotype_id-2]=I;
                    DGG[current_haplotype_id-2]=D;
                }

                __syncthreads();
            }
        }


        if(threadIdx.x==round_size-1)
        {
            result[offset]=result_block;
        }
        offset+=gridDim.x;
    }
}

int runBatch(Batch& batch)
{

    float * MG;
    float * DG;
    float * IG;
    cudaMalloc( (float **)& MG,sizeof(float) *240*500*3);
    DG=MG+240*500;
    IG=DG+240*500;

    int INI=(log10f((std::numeric_limits<float>::max() / 16)));

    int size;

    // ph2pr_h
    float ph2pr_h[BSIZE];
    for(int i=0;i<BSIZE;i++)
    {
        ph2pr_h[i]=powf(10.f, -((float)i) / 10.f);
    }


    while(!feof(file))
    {

        InputData *inputdata=(InputData* )malloc(size*(sizeof(InputData)));

        size = batch.num_reads * batch.num_haps;

        // align to BSIZE
        int mem_size=BALIGN(size*260); //read_base
        mem_size+=BALIGN(size*500); // haplotyp_base;
        mem_size+=BALIGN(size*260*4);//parameter1;
        mem_size+=BALIGN(size*260*4);//parameter2;
        mem_size+=BALIGN(size*260*4);//parameter3;
        mem_size+=BALIGN(size*260*4);//parameter4;
        mem_size+=BALIGN(size*260*4);//result;
        //mem_size+=(size* sizeof(float)+BSIZE-1)/BSIZE*BSIZE;//result;
        mem_size+=BALIGN(size*sizeof(NUM_ADD));//NUM_ADD;

        char *data_h_total=(char*)malloc(mem_size); //on cpu host

        // device
        char * result_d_total;
        cudaError err;
        err=cudaMalloc( (char **) &result_d_total, mem_size);
        if(err!=cudaSuccess)
            printf( "Error %d: %s!\n", err, cudaGetErrorString(err) );

        char * data_d_total=result_d_total+BALIGN(size*sizeof(float));  //on GPU

        char * data_h=data_h_total;
        char * data_h_begin=data_h;  //cpu
        NUM_ADD *data_num_add=(NUM_ADD *) (data_h); //cpu

        data_h=data_h+BALIGN(size*sizeof(NUM_ADD));

        int data_size=0;
        for(int i=0;i<size;i++)
        {
            int hi = i/batch.num_reads; // haplotype index
            int read_size=batch.reads[i].length;

            // parameter
            int skip=BALIGN(sizeof(float)*read_size)/sizeof(float);
            float parameter[1040];  // 260*4
            for(int j=0;j<read_size;j++)
            {
                parameter[j]= ph2pr_h[batch.reads[i].bases[j]&127 ];     //QM
                parameter[j+skip]=ph2pr_h[batch.reads[i].i[j]&127];      //Qi
                parameter[j+skip*2]=ph2pr_h[batch.reads[i].d[j]&127];    //QD
                parameter[j+skip*3]=1.0f-ph2pr_h[((int)(batch.reads[i].i[j]&127)+(int)(batch.reads[i].d[j]&127))&127];  // c
            }

            // haplotype
            int haplotype_new_size=(batch.haps[hi].length + 4-1)/4;
            char4 haplotype_base_new[150];
            for(int j=0;j<haplotype_new_size;j++)
            {
                haplotype_base_new[j].x = batch.haps[hi].bases[j*4];

                if(j*4+1 < batch.haps[hi].length)
                    haplotype_base_new[j].y = batch.haps[hi].bases[j*4+1];

                if(j*4+2 < batch.haps[hi].length)
                    haplotype_base_new[j].z = batch.haps[hi].bases[j*4+2];

                if(j*4+3 < batch.haps[hi].length)
                    haplotype_base_new[j].w = batch.haps[hi].bases[j*4+3];
            }

            // data_num_add on cpu
            data_num_add[i].read_haplotype_number.x=batch.reads[i].length;
            data_num_add[i].read_haplotype_number.y=batch.haps[hi].length; //inputdata[i].haplotype_size;
            data_num_add[i].address_array=data_size;

            char read_base_new[260];
            for(int j=0;j<read_size;j++)
            {
                read_base_new[j]=inputdata[i].read_base[j];
            }
            //read base
            memcpy(data_h,read_base_new,sizeof(char)*read_size);
            data_h+=(read_size+BSIZE-1)/BSIZE*BSIZE;
            data_size+=(read_size+BSIZE-1)/BSIZE*BSIZE;

            //parameter
            memcpy(data_h,parameter,sizeof(float) *skip*4);
            data_h+=sizeof(float) *skip*4;
            data_size+=sizeof(float) *skip*4;

            //haplotype
            memcpy(data_h,haplotype_base_new,sizeof(char4)* haplotype_new_size);
            data_h+=(haplotype_new_size*sizeof(char4)+BSIZE-1)/BSIZE*BSIZE;
            data_size+=(haplotype_new_size*sizeof(char4)+BSIZE-1)/BSIZE*BSIZE;
        }

        int data_size_to_copy=data_size+(size*sizeof(NUM_ADD)+BSIZE-1)/BSIZE*BSIZE;
        char * data_d;
        float * result_d=(float *) (result_d_total);
        int blocksize=BSIZE;
        int gridsize=180;
        NUM_ADD * num_add_d=(NUM_ADD *) (data_d_total);
        data_d=data_d_total+(sizeof(NUM_ADD)*size+BSIZE-1)/BSIZE*BSIZE;

        clock_gettime(CLOCK_MONOTONIC_RAW,&start);
        err=cudaMemcpy(data_d_total,data_h_begin,data_size_to_copy,cudaMemcpyHostToDevice);
        if(err!=cudaSuccess)
            printf( "Error %d: %s!\n", err, cudaGetErrorString(err) );
        pairHMM<<<gridsize,blocksize>>> (size,data_d,num_add_d,result_d,MG,DG,IG);
        //cudaDeviceSynchronize();
        cudaMemcpy (batch.results,result_d_total,size*sizeof(float),cudaMemcpyDeviceToHost);
        clock_gettime(CLOCK_MONOTONIC_RAW,&finish);
        computation_time+=diff(start,finish);

        free(data_h_total);
        err=cudaFree(result_d_total);
        if(err!=cudaSuccess)
            printf( "Error %d: %s!\n", err, cudaGetErrorString(err) );

        for(int i=0;i<size;i++)
            float aa=(log10f((double)batch.results[i]) - INI);

        free(inputdata);
        free(result_h);
        fscanf(file,"%d",&size);
        //	if(total>10000)
//			break;
    }//end of while

    clock_gettime(CLOCK_MONOTONIC_RAW,&start);
    cudaFree(MG);
    cudaDeviceReset();
    clock_gettime(CLOCK_MONOTONIC_RAW,&finish);
    //mem_cpy_time+=diff(start,finish);//(finish1.tv_nsec-start1.tv_nsec)/1000000000.0;

    //	printf("size %d\n",total);
    printf("read_time=%e  initial_time=%e  computation_time= %e total_time=%e\n",read_time, data_prepare,computation_time, computation_time+mem_cpy_time+data_prepare);
    //	printf("%d %d %d  %e\n", fakesize, read_read, haplotype_haplotype,computation_time);
    printf("Total time=%e\n",total_time);
    return 0;
}

bool calculate(Batch& batch)
{

}