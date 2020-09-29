#include <stdio.h>
#include <string.h>/*For strlen*/
#include <mpi.h> /* For MPI functions, etc */

//const int MAX_STRING = 100;
const int PING_POG_LIMIT = 10;


int main(int argc, char **argv) {
	int		comm_sz; /* Number of processes */
	int 	my_rank; /* My process rank */

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);




	int ping_pong_count=0;
	int parther_rank=(my_rank +1)%2;
	while(ping_pong_count < PING_POG_LIMIT){
		if(my_rank ==ping_pong_count%2){
			ping_pong_count++;
			MPI_Send(&ping_pong_count, 1, MPI_INT, parther_rank, 0,MPI_COMM_WORLD);
			printf("%d envia y aumenta el contador pingpong %d a %d\n",my_rank,ping_pong_count,parther_rank);

		}
		else{
		MPI_Recv(&ping_pong_count, 1, MPI_INT, parther_rank, 0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("%d recibe el contador pingpong %d de %d\n",my_rank,ping_pong_count,parther_rank);


		}

	}

	MPI_Finalize();
	return 0;
}	/*main */
