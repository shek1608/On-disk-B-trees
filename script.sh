gcc -o assn_4 main.c -lm


echo "Testing input-01a.txt"
rm index.bin

./assn_4 index.bin 4 < ./test/testcases/input-01a.txt > myoutput_01a.txt
diff ./test/testcases/output-01a.txt myoutput_01a.txt

echo "Testing input-02a.txt"

./assn_4 index.bin 4 < ./test/testcases/input-02a.txt > myoutput_02a.txt
diff ./test/testcases/output-02a.txt myoutput_02a.txt

rm index.bin

echo "Testing input-03.txt"

./assn_4 index.bin 3 < ./test/healey/input-03.txt > myoutput_03.txt
diff ./test/healey/output-03-first.txt myoutput_03.txt

rm index.bin

echo "Testing input-04.txt"

./assn_4 index.bin 4 < ./test/healey/input-04.txt > myoutput_04.txt
diff ./test/healey/output-04-first.txt myoutput_04.txt

rm index.bin

echo "Testing input-20.txt"

./assn_4 index.bin 20 < ./test/healey/input-20.txt > myoutput_20.txt
diff ./test/healey/output-20-first.txt myoutput_20.txt

rm index.bin

echo "Testing input-10-01.txt"

./assn_4 index.bin 10 < ./test/healey/input-10-01.txt > myoutput_10_01.txt
diff ./test/healey/output-10-01.txt myoutput_10_01.txt

echo "Testing input-10-02.txt"

./assn_4 index.bin 10 < ./test/healey/input-10-02.txt > myoutput_10_02.txt
diff ./test/healey/output-10-02.txt myoutput_10_02.txt


echo "----Testing Graded Folder------"

rm index.bin

echo "Testing input-03.txt"

./assn_4 index.bin 3 < ./test/graded/input-03.txt > myoutput_03_graded.txt
diff ./test/graded/output-03.txt myoutput_03_graded.txt

rm index.bin

echo "Testing input-04.txt"

./assn_4 index.bin 4 < ./test/graded/input-04.txt > myoutput_04_graded.txt
diff ./test/graded/output-04.txt myoutput_04_graded.txt


rm index.bin
echo "Testing input-20.txt"

./assn_4 index.bin 20 < ./test/graded/input-20.txt > myoutput_20_graded.txt
diff ./test/graded/output-20.txt myoutput_20_graded.txt

rm index.bin
echo "Testing input-10-01.txt"

./assn_4 index.bin 10 < ./test/graded/input-10-01.txt > myoutput_10_01_graded.txt
diff ./test/graded/output-10-01.txt myoutput_10_01_graded.txt

echo "Testing input-10-02.txt"

./assn_4 index.bin 10 < ./test/graded/input-10-02.txt > myoutput_10_02_graded.txt
diff ./test/graded/output-10-02.txt myoutput_10_02_graded.txt


echo "--------TESTING RANDOM------------"
rm index.bin
echo "in-1000-rand.txt order 4"
./assn_4 index.bin 4 < ./test/random/in-1000-rand.txt > my-1000-rand-4.txt
diff my-1000-rand-4.txt ./test/random/out-1000-rand-order4.txt


rm index.bin
echo "in-1000-rand.txt order 10"
./assn_4 index.bin 10 < ./test/random/in-1000-rand.txt > my-1000-rand-10.txt
diff my-1000-rand-10.txt ./test/random/out-1000-rand-order10.txt

rm index.bin
echo "in-1000-rand.txt order 32"
./assn_4 index.bin 32 < ./test/random/in-1000-rand.txt > my-1000-rand-32.txt
diff my-1000-rand-32.txt ./test/random/out-1000-rand-order32.txt

rm index.bin
echo "in-1000-seq.txt order 4"
./assn_4 index.bin 4 < ./test/random/in-1000-seq.txt > my-in-1000-seq.txt
diff my-in-1000-seq.txt ./test/random/out-1000-seq-order4.txt


rm index.bin
echo "in-10k-rand.txt order 10"
./assn_4 index.bin 10 < ./test/random/in-10k-rand.txt > my-in-10k-rand.txt
diff my-in-10k-rand.txt ./test/random/out-10k-rand-order10.txt


rm index.bin
echo "in-10k-rand.txt order 32"
./assn_4 index.bin 32 < ./test/random/in-10k-rand.txt > my-in-10k-rand32.txt
diff my-in-10k-rand32.txt ./test/random/out-10k-rand-order32.txt

rm index.bin

echo "in-500-seq-1.txt order 4"
./assn_4 index.bin 4 < ./test/random/in-500-seq-1.txt > my-in-500-seq-1.txt
diff my-in-500-seq-1.txt ./test/random/out-500-seq-order4-1.txt

echo "in-500-seq-2.txt order 4"
./assn_4 index.bin 4 < ./test/random/in-500-seq-2.txt > my-in-500-seq-2.txt
diff my-in-500-seq-2.txt ./test/random/out-500-seq-order4-2.txt



