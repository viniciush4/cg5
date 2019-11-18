class Pista {
public:
	float x1;
	float y1;
	float x2;
	float y2;

	Pista();
	Pista(float x1, float y1, float x2, float y2);
	void desenhar();
	virtual ~Pista();
};
