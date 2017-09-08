#ifndef _PAGINAH_
#define _PAGINAH_

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <set>


using namespace std;

int ContarTags(string cadena);


class pagina{
private:
  vector<string> fuente;
  vector<string> codificado;
  map<string,string> tag_basicos;
  map<string,string> literal_basicos;
  map<string,string> literal_notag;
  map<string,string> literal_tag;
  map<string,string> otros_tag;
  map<int,string> literal_tag_ord;
  map<int,string> literal_notag_ord;
  map<string,int> palabras_clave;
  map<string,string> literalprueba;


public:

  pagina();
  pagina(const char *nomfich);

  string Sacar_Opcion_Tag(string opcion, int linea);
  string Sacar_Value(int linea);
  int Buscar_Patron(string patron, int &next, vector<string> & value);

  void Lista_Fuente(int inicio, int fin);
  string Lista_Fuente_String(int inicio, int fin);
  void Lista_Codificado();
  void Lista_Tag_Basico();
  void Lista_Otros_Tag();
  void Lista_Literal_Basico();
  void Lista_Literal_Notag();
  void Lista_Literal_Tag();
  void Sacar_http(list<string> &cola, set<string> &images, bool &contenido_relevante,  map<string,int> &palabras_clave, string &palabra_clave_encontrada);

};


#endif
