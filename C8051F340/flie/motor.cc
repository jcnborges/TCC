#include "motor.h"

TTipoPremissa::TTipoPremissa()
{
}

TTipoPremissa::TTipoPremissa(char *newAtributo, float newV)
{
   atributo = newAtributo;
   valor = newV;
   tamvp=0;


}
void TTipoPremissa::setValor(float newValor)
{
   valor = newValor;
}

TVetor *TTipoPremissa::getVPremissas()
{
   return &vPremissas;
}

int TTipoPremissa::getValorPremissa(int premissa)
{
    return vPremissas[premissa];
}

int TTipoPremissa::getTamvp()
{
   return tamvp;
}

void TTipoPremissa::inserePremissa(int premissa)
{
   vPremissas[tamvp] = premissa;
   tamvp++;
}

void TTipoPremissa::Notificacao()
{
}

/* INICIO DA IMPLEMENTAÇÃO DA CLASSE TCondicao 	*/
TCondicao::TCondicao(float newV, float newVR)
{
    valor = newV;
    valorRecebido = newVR;
}

void TCondicao::setValorRecebido(float newVR)
{
    valorRecebido = newVR;
}

bool TCondicao::Maior() //tipo (1)
{
    if (valorRecebido > valor){
	return true;
    }
    else
    {
	return false;
    }
}

bool TCondicao::Menor() //tipo (2)
{
    if (valorRecebido < valor) {
       return true;
    }
    else {
       return false;
    }
}

bool TCondicao:: Igual() //tipo (3)
{
    if (valorRecebido == valor) {
       return true;
    }
    else {
       return false;
    }

}

TCondicao::~TCondicao()
{
}




/* FIM DA IMPLEMENTA€ÇO DA CLASSE TCondicao 	*/


TPremissa::TPremissa():condicao(0,0)
{
}

TPremissa::TPremissa (Qual newTipo, char *newAtributo, float newV, float newVR, bool newStatus): condicao(newV,newVR)
{
      tipo = newTipo;
      atributo = newAtributo;
      status = newStatus;
      tamvr =0;
      for (int i=0; i <10; i++)
	 vRegras[i] =0;
}

void TPremissa::setStatus (float valorRecebido)
{
      condicao.setValorRecebido(valorRecebido);
      switch (tipo)
      {
	  case Maior: status = condicao.Maior();break;
	  case Menor: status = condicao.Menor();break;
	  case Igual: status = condicao.Igual();break;
	  default : status = false;
	  /*eh necess rio rever os tipos de condicoes existentes,
	  pode ser necessario criar mais um tipo, talves pertence
	  ou nao pertence devido a mudan‡a da comparacao*/
      }
}

bool TPremissa::getStatus()
{
      return status;
}

Qual TPremissa::getTipo()
{
     return tipo;
}

TVetor *TPremissa::getVRegras()
{
     return &vRegras;
}

void TPremissa::InsereRegras(int regra)
{
    vRegras[tamvr] = regra;
    tamvr++;
}

int TPremissa::getValorRegra(int regra)
{
    return vRegras[regra];
}

int TPremissa::getTamvr()
{
   return tamvr;
}

void TPremissa::Notifica()
{
}


TConclusao::TConclusao(char *newMensagem, bool newStatus)
{
    mensagem = newMensagem;
    status = newStatus;
}

char * TConclusao::getMensagem()
{
    return mensagem;
}

TVetor * TConclusao::getVRegras()
{
    return &vRegras;
}

void TConclusao::setStatus (bool newStatus)
{
    status = newStatus;
}

bool TConclusao::getStatus()
{
   return status;
}


TRegra::TRegra()
{
   nome = "";
   tamvp =0;
   tamc =0;
}

TRegra::TRegra(char * newNome)
{
   nome = newNome;
   tamvp =0;
   tamc =0;
}
char * TRegra::getNome()
{
   return nome;
}

TVetor * TRegra::getPremissas()
{
   return &vPremissas;
}
TVetor * TRegra::getConclusao()
{
   return &conclusao;
}

int TRegra::getTamvp()
{
   return tamvp;
}
int TRegra::getTamc()
{
   return tamc;
}
void TRegra::InserePremissa(int Premissa)
{
   vPremissas[tamvp] = Premissa;
   tamvp++;
}

void TRegra::InsereConclusao(int newConclusao)
{
   conclusao[tamc] = newConclusao;
   tamc++;
}

int TRegra::getValorPremissa(int premissa)
{
   return vPremissas[premissa];
}

int TRegra::getValorConclusao()
{
   return conclusao[0];
}
void TRegra::Notifica()
{
}

