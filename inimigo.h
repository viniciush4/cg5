class Inimigo {
public:
	float x;
	float y;
	float z;
	float r;
	float angulo_xy;
	float angulo_xz;
	float angulo_canhao_xy;
	float angulo_canhao_xz;
	float angulo_canhao_arena_xy;
	float angulo_canhao_arena_xz;
	float angulo_helices;
	float velocidade;
	float somatorio_incremento_angulo;
	bool incrementar_angulo;
	float tempo_desde_ultimo_tiro;

	Inimigo();
	Inimigo(float x, float y, float z, float r, float velocidade);
	void desenhar();
	void alterarAngulo(float incremento_xy, float incremento_xz);
	void andar(float coeficiente_ajuste);
	void girarHelices(float coeficiente_ajuste);
	
	virtual ~Inimigo();
};
