echo "./atpg -bt 1 -output b17_vectors -check_point b17.bench "
./atpg -bt 1 -output b17_1_cp_vectors -check_point b17.bench 
echo ""

echo "./atpg -bt 10 -output b17_vectors -check_point b17.bench "
./atpg -bt 10 -output b17_10_cp_vectors -check_point b17.bench 
echo ""

echo "./atpg -bt 100 -output b17_vectors -check_point b17.bench "
./atpg -bt 100 -output b17_100_cp_vectors -check_point b17.bench 
echo ""

echo "./atpg -bt 1000 -output b17_vectors -check_point b17.bench "
./atpg -bt 1000 -output b17_1000_cp_vectors -check_point b17.bench 

echo "./atpg -output b17_vectors b17.bench "
./atpg -output b17_vectors b17.bench 

echo "./atpg -output b17_vectors -check_point b17.bench "
./atpg -output b17_cp_vectors -check_point b17.bench 

echo ""

