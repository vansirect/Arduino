#include <stdio.h>
#define N 40

void fibsequence(int fib_numbers[]) {

	fib_numbers[0] = 0;
	fib_numbers[1] = 1;
	for (int i = 2; i < N;i++) {
		fib_numbers[i] = fib_numbers[i - 1] + fib_numbers[i - 2];
	}
}




int main() {
	int fib_numbers[N];
	fibsequence(fib_numbers);
	for (int j = 0;j < N;j++) {

		printf("%d ", fib_numbers[j]);

	}
	


}