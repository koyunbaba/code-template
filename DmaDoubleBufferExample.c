#define	N				16
#define BUF_IN_SIZE		1024
#define BUF_OUT_SIZE	1024
#define DDR_IN_SIZE		(BUF_IN_SIZE * N)
#define DDR_OUT_SIZE	(BUF_OUT_SIZE *N)

// Memory in RAM
int ram_in[BUF_IN_SIZE * 2];
int ram_out[BUF_OUT_SIZE * 2];

// Memory in DDR
int ddr_in[DDR_IN_SIZE];
int ddr_out[DDR_OUT_SIZE];

void DmaDoubleBufferExample(void)
{
	int	in_idx, out_idx;
	int i;

	in_idx = 0;
	out_idx = 0;

	DmaMemCpyToRam(&ddr_in[0], &ram_in[in_idx * BUF_IN_SIZE], BUF_IN_SIZE);

	for(i = 1; i < N; i++)
	{		
		// data-in / processing / data-out are working simultaneously here!
		
		in_idx ^= 1;
		
		DmaWaitDone(DDR2RAM);
		DmaMemCpyToRam(&ddr_in[BUF_IN_SIZE * i], &ram_in[in_idx * BUF_IN_SIZE], BUF_IN_SIZE);
				
		DoSomething(&ram_in[out_idx * BUF_IN_SIZE], &ram_out[out_idx * BUF_OUT_SIZE], BUF_IN_SIZE, BUF_OUT_SIZE);
		
		DmaWaitDone(RAM2DDR);
		DmaMemCpyToDdr(&ram_out[out_idx * BUF_OUT_SIZE], &ddr_out[BUF_OUT_SIZE * (i - 1)], BUF_OUT_SIZE);
		
		out_idx ^= 1;
	}

	DoSomething(&ram_in[out_idx * BUF_IN_SIZE], &ram_out[out_idx * BUF_OUT_SIZE], BUF_IN_SIZE, BUF_OUT_SIZE);
	
	DmaWaitDone(RAM2DDR);
	DmaMemCpyToDdr(&ram_out[out_idx * BUF_OUT_SIZE], &ddr_out[BUF_OUT_SIZE * i], BUF_OUT_SIZE);
}
