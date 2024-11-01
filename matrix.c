#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int n, **matrix1, **matrix2, **resultantMatrix;

void *takeUserInput(void *args)
{
    int **ptr = (int**)args;

    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0; j < n ; j++)
        {
            scanf("%d",&ptr[i][j]);
        }
    }
}

void *printMatrix(void *args)
{
    int **ptr = (int **)args;
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0; j < n ; j++)
        {
            printf("%d ",ptr[i][j]);
        }
        printf("\n");
    }
}

void *calculateAddition()
{
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            resultantMatrix[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

void *calculateSubtraction()
{
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            resultantMatrix[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
}

void *calculateMultiplication()
{
    int sum = 0;
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            for(int k = 0 ; k < n ; k++)
            {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            resultantMatrix[i][j]=sum;
            sum = 0;
        }
    }


}

void main()
{
    pthread_t t1,t2, threadForAddition, threadForSubtraction, threadForMatrixMultiplication;


    printf("Enter the number of rows and columns for all matrices:");
    scanf("%d",&n);

    matrix1 = (int **)malloc(sizeof(int *)*n);
    matrix2 = (int **)malloc(sizeof(int *)*n);
    resultantMatrix = (int **)malloc(sizeof(int *)*n);

    for(int i = 0 ; i < n ; i++)
    {
        matrix1[i] = (int *)malloc(sizeof(int)*n);
        matrix2[i] = (int *)malloc(sizeof(int)*n);
        resultantMatrix[i] = (int *)malloc(sizeof(int)*n);
    }

    printf("\nEnter details for matrix 1:\n");
    pthread_create(&t1,NULL,takeUserInput,matrix1);
    pthread_join(t1,NULL);


    printf("\nEnter details for matrix 2:\n");
    pthread_create(&t2,NULL,takeUserInput,matrix2);
    pthread_join(t2,NULL);


    printf("\nMatrix 1:\n");
    pthread_create(&t1,NULL,printMatrix,matrix1);
    pthread_join(t1,NULL);

    printf("\nMatrix 2:\n");
    pthread_create(&t2,NULL,printMatrix,matrix2);
    pthread_join(t2,NULL);


    pthread_create(&threadForAddition,NULL,calculateAddition,NULL);
    pthread_join(threadForAddition,NULL);
    printf("\nAddition of matrices:\n");
    pthread_create(&threadForAddition,NULL,printMatrix,resultantMatrix);
    pthread_join(threadForAddition,NULL);

    pthread_create(&threadForSubtraction,NULL,calculateSubtraction,NULL);
    pthread_join(threadForSubtraction,NULL);
    printf("\nSubtraction result:\n");
    pthread_create(&threadForSubtraction,NULL,printMatrix,resultantMatrix);
    pthread_join(threadForSubtraction,NULL);


    pthread_create(&threadForMatrixMultiplication,NULL,calculateMultiplication,NULL);
    pthread_join(threadForMatrixMultiplication,NULL);
    printf("Matrix Multiplication:\n");
    pthread_create(&threadForMatrixMultiplication,NULL,printMatrix,resultantMatrix);
    pthread_join(threadForMatrixMultiplication,NULL);

    for(int i = 0 ; i < n ;i++)
    {
        free(matrix1[i]);
        free(matrix2[i]);
        free(resultantMatrix[i]);
    }


    free(matrix1);
    free(matrix2);
    free(resultantMatrix);


}