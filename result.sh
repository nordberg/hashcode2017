clang++ -std=c++14 -O3 main.cpp
zip out.zip main.cpp knapsack.hpp
echo "Created out.zip"
FILES="me_at_the_zoo.in trending_today.in videos_worth_spreading.in kittens.in"
ARGS="$@"
for input in ${ARGS:-$FILES}
do
	echo "$input""…"
	./a.out $input > ${input/.in/.out}
done
