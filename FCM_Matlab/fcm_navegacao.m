clear
posicao=17;
A=1; % default
SDN(1)=0;
SEN(1)=0;
SFN(1)=0;
ace=1; % amplitude do movimento.
x1f=0;
xp=0;
SensorE(1)=0; % acumula a somatória dos efeitos dos tres obstaculos nos sensores
SensorD(1)=0;
SensorF(1)=0;
fe=1;
fd=1;
x5=12;
    y5=66;
     e(1)=0;
d(1)=0;
final=200;
z=0:0.28:148; % ajusta a velocidade de animação 
nz=size(z,2);
if z>20
    ace=5;
end
% equaçao de um sistema massa mola do livro equaçoes diferenciais pg 218
shading interp
colormap(copper)
% transicao dos tipos de movimento. segundo estado da maquina, o salto
   x1=20+z/100000000000000000000000;
y1=z*ace+5;
for i=1:nz
		 % fcm tomadad de decisao on line continuo relacoes causais 
		 % regras fuzzy para tomada de decisoes eventuais 
		 % regras para adaptação do fcm 
		 % regras fuzzy para decisoes do tipo realimentacao 
		 % regras de sobrevivivcencia 
			 % conceitos de entrada -
			 % objeto a esquerda !!!
			 % Objeto a direita - 
			 % Objeto em linha reta 
			 % calculo da distancia do robo e o objeto. 
		% obstaculo 1 ( 17,38)      
	SensorDN(i)=SensorD(i)/3;
	SensorFN(i)=SensorF(i)/3;
	SensorEN(i)=SensorE(i)/3;
	% conceito 1 - andar linha reta ( sem obstaculos o robo acelera ) 
	xx=SensorFN(i)*0.5;
	ACELERA(i)=(1/(1+exp(-xx))); % atualizacao 
	% conceito 2 -  virar esquerda 
	xx=(SensorDN(i)*0.5+SensorFN(i)*0.8); % Ok 
	VE(i)=(1/(1+exp(-xx))); % funcao sigmoide 
	% conceito 3 - virar direita 
	xx=(SensorEN(i)*0.5+SensorFN(i)*0.8);
	VD(i)=(1/(1+exp(-xx)));
		% bloco de tomada de decisoes ( inferencia a atraves de variaveis
		% linguisticas !!!!!
		% a principio as entradas do sensor sao discretizadas em cinco niveis 
		% muito forte  [0.8 a 1 )
		% forte  [0.6 a 0.8 )
		% medio [0.4 a 0.6)
		% fraco [0.2 a 0.4)
		% muito fraco [0 a 0.2) 
	 e(i)=0;
	 d(i)=0;
	 
	 if ACELERA(i)>0.55
		 A=0.92*1/(ACELERA(i)*2);
	 else 
		 A=1.03*1/(ACELERA(i)*2);
	 end 
	 
	 if VE(i)>0.52
		 e(i)=(21*VE(i)+e(i-1))/2; % o conceito pode ser usado como uma memoria de movimento, suavizando as tomadas de decisoes
		 e(i)=e(i)*fe;
		 me=1;
		 fd=0;
		 A=0.87;
	 else 
		 me=0;
		 fd=fd+0.4;
	 end
	 
	 if me>0 
		 if VD(i)>0.515
			 fd=fd+0.05;
		 end
	 end
	 
	 if fd>1
		 fd=1;
	 end
	 
	 if VD(i)>0.52
		 d(i)=(21*VD(i)+d(i-1))/3;
		 d(i)=d(i)*fd;
		 fe=0;
		 md=1;
		 A=0.87;
	 else
		 md=0;
		 fe=fe+0.4;
	 end

	 if md>0 
		 if VE(i)>0.515
			 fe=fe+0.05;
		 end
	 end

	 if fe>1
		 fe=1;
	 end

	if i>1
		if e(i)<e(i-1)
			posicao=x1(i-1);
		end

		if d(i)<d(i-1)
			posicao=x1(i-1);
		end % rotina de atualização de posição !!!
	end

	vira=or(e(i),d(i));

	if vira==0
		x1(i)=posicao;
		y1(i)=-2+A*y1(i);
		x2(i)=-2+x1(i);
		y2(i)=y1(i);
		x3(i)=-4+x1(i);
		y3(i)=y1(i);
		x4(i)=17;
		y4(i)=40;
		x5(i)=10;
		y5(i)=100;
		x5(5)=7;
		y5(5)=140;
		x5(6)=15;
		y5(6)=66;
		x5(7)=-7;
		y5(7)=150;
		x6(i)=5;
		y6(i)=120;
	end
	% movimento pra direita 
	if d(i)>0
		x1(i)=posicao+d(i)/12.5;
		xp=1;
		y1(i)=-2+y1(i);
		x2(i)=-2+x1(i);
		y2(i)=+d(i)/4+y1(i);
		x3(i)=-4+x1(i);
		y3(i)=+d(i)/2+y1(i);
		x4(i)=17;
		y4(i)=40;
		x5=6;
		y5=140;
	end 
	% movimento pra esquerda
	if e(i)>0 
		x1(i)=posicao-e(i)/12.5;
		y1(i)=-2+y1(i);
		x2(i)=-2+x1(i);
		y2(i)=-e(i)/4+y1(i);
		x3(i)=-4+x1(i);
		y3(i)=-e(i)/2+y1(i);
		x4=18;
		y4=38;
		x5=15;
		y5=66;
		x6=5;
		y6=120;
		x7=-9;
		y7=140;
	end
	for k=1:8
		% coordenadas dos obstaculos
		Xo(1)=19;
		Yo(1)=35;
		Xo(2)=19;
		Yo(2)=38;
		Xo(3)=15;
		Yo(3)=66;
		Xo(4)=10;
		Yo(4)=100;
		Xo(5)=5;
		Yo(5)=120;
		Xo(6)=-7;
		Yo(6)=150;
		Xo(7)=-9;
		Yo(7)=140;
		Xo(8)=7;
		Yo(8)=140;
		% tratar a dificuldade de desviar de dois obstaculos !!!  Problema
		% comeca a refinar tratado com tomada de decisoes baseadas em eventos,
		% modelados por conceitos e relacoes de selecao e um conceito de fator
		% no momento da acao da curva.
		dx(i)=(x1(i)-Xo(k));% sensor direita 
		dy(i)=(y1(i)-Yo(k));
		dist=(dx(i)^2+dy(i)^2)^0.5;
		dx2(i)=(x2(i)-Xo(k));% sensor frontal
		dy2(i)=(y2(i)-Yo(k));
		dist2=(dx2(i)^2+dy2(i)^2)^0.5;
		dx3(i)=(x3(i)-Xo(k));% sensor esquerda 
		dy3(i)=(y3(i)-Yo(k));
		dist3=(dx3(i)^2+dy3(i)^2)^0.5;
		SEN(k)=0; % inicializa os valores normalizados do sensor para nao acumular valores passo anterior
		SDN(k)=0;
		SFN(k)=0;
		% SENSOR LATERAL ESQUERDO
		if dist3<=11 % alcance do sensor (menos que isso nao capta nada)    
			if dx3(i)>0    
				if dy3(i)<0
					SE(i)= 1/(dx3(i)^2+dy3(i)^2)^0.5;
					SEN(k)=SE(i)/max(SE);  % somatorio final
					% intensidade do sensor é inversamente proporcional a distancia
				else 
					SEN(k)=0;
				end
			end
		else 
		   SEN(k)=0;
		end
		% SENSOR LATERAL DIREITO
		if  dist<=11
			if dx(i)<0    
				if dy(i)<0
					SD(i)= 1/(dx(i)^2+dy(i)^2)^0.5;
					SDN(k)=SD(i)/max(SD);
				else
					SDN(k)=0;
				end
			end
		else
			SDN(k)=0;
		end 
		% sensor frontal
		if dist2<=11
			if dy2(i)<0
				if dx2(i)<=1 
					SF(i)=1/(dx2(i)^2+dy2(i)^2)^0.5;
					SFN(k)=SF(i)/max(SF);
				end
				if dx2(i)<=-1 
					SF(i)=1/(dx2(i)^2+dy2(i)^2)^0.5;
					SFN(k)=SF(i)/max(SF);
				end
			else
				SFN(k)=0;
			end
		else
			SFN(k)=0;
		end
	end
	SensorE(i+1)=sum(SEN); % acumula a somatória dos efeitos dos tres obstaculos nos sensores
	SensorD(i+1)=sum(SDN);
	SensorF(i+1)=sum(SFN);
end
for i=1:8
    x5(i)=Xo(i);
    y5(i)=Yo(i);
end
for i=1:nz-6
    shading interp
    plot(x1(i),y1(i),'*',x2(i),y2(i),'*',x3(i),y3(i),'*',x5,y5,'+',x1(i),y1(i+5),'*',x2(i),y2(i+5),'+',x3(i),y3(i+5),'*')
    axis([-50 70  0 final]);
    mov(i)=getframe;
end
