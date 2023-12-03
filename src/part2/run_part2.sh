	echo "----------Part 2 -------------- "
	make
	# parallel-scp -h ~/HW4/src/part2/hosts -r ~/HW4/src/part2 ~/HW4/src/part2
	mpirun -np 4 --hostfile ./hosts matmul < sample_input


	# echo "----------Part 2: 只在本地端跑 -------------- "
	# make clean
	# make
	# mpiexec -n 2  matmul < sample_input

	
	# echo "----------Part 2: 只在本地端跑 -------------- "
	# make clean
	# make
	# mpiexec -n 2  matmul < ../../data-set/data1_1 >./my_answer/ans1_1