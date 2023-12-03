# echo "------------------------ "
# echo "Part 1-2-1 Linear"
# mpicxx ~/HW4/src/pi_block_linear.cc -o ~/HW4/src/pi_block_linear;
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/src ~
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/ref ~
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_block_linear 1000000000 
# echo "------------------------ "
# echo "TA's program:"
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_block_linear 1000000000 


echo "------------------------ "
echo "Part 1-2-2 Binary tree"
mpicxx ~/HW4/src/pi_block_tree.cc -o ~/HW4/src/pi_block_tree;
parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4 ~
mpirun -np 16 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_block_tree 1000000000 
echo "------------------------ "
echo "TA's program:"
mpirun -np 16 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_block_tree 1000000000

# echo "------------------------ "
# echo "Part 1-2-3 non-bocking Linear"
# mpicxx ~/HW4/src/pi_nonblock_linear.cc -o ~/HW4/src/pi_nonblock_linear;
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/src ~
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/ref ~
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_nonblock_linear 1000000000 
# echo "------------------------ "
# echo "TA's program:"
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_nonblock_linear 1000000000  

# echo "------------------------ "
# echo "Part 1-2-4 gather "
# mpicxx ~/HW4/src/pi_gather.cc -o ~/HW4/src/pi_gather;
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/src ~
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/ref ~
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_gather 1000000000
# echo "------------------------ "
# echo "TA's program:"
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_gather 1000000000

# echo "------------------------ "
# echo "Part 1-2-5 reduce "
# mpicxx ~/HW4/src/pi_reduce.cc -o ~/HW4/src/pi_reduce;
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/src ~
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/ref ~
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_reduce 1000000000
# echo "------------------------ "
# echo "TA's program:"
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_reduce 1000000000

# echo "------------------------ "
# echo "Part 1-2-6 one_side "
# mpicxx ~/HW4/src/pi_one_side.cc -o ~/HW4/src/pi_one_side;
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/src ~
# parallel-scp -h ~/HW4/src/hosts.txt -r ~/HW4/ref ~
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/src/pi_one_side 1000000000
# echo "------------------------ "
# echo "TA's program:"
# mpirun -np 4 --hostfile ~/HW4/src/hosts.txt ~/HW4/ref/pi_one_side 1000000000