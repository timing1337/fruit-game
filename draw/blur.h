
class Kernel {
public:
	int radius;
	int div;
	int* dv;
	Kernel(int rad) {
		radius = rad;
		div = rad * 2 + 1;
		dv = new int[256 * div];

		for (int i = 0; i < 256 * div; i++) {
			dv[i] = (i / div);
		}
	};
};