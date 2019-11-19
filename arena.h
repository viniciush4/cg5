class Arena {
public:
	float x;
	float y;
	float r;
	float altura;

	Arena();
	Arena(float x, float y, float r, float altura);
	void desenhar();
	
	virtual ~Arena();
};
