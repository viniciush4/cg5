class Base {
public:
	float x;
	float y;
	float r;

	Base();
	Base(float x, float y, float r);
	void desenhar();
	
	virtual ~Base();
};
