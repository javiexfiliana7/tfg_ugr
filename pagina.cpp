#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <queue>
#include <fstream>
#include "pagina.h"
#include <string>

using namespace std;

pagina::pagina(){
}

void Pausa(){
  char ch;
  cout << "Pulsa una tecla ..." << endl;
  cin >> ch;
}

void RecursivoParentesis(string &cadena, ifstream &f){
  char caracter;
  bool salir = false;

  while (!f.eof() and !salir){
    caracter = f.get();
    cadena += caracter;
    if (caracter == '('){
      RecursivoParentesis(cadena,f);
    }
    else if (caracter == ')'){
      salir = true;
    }
  }
}


void RecursivoLlaves(string &cadena, ifstream &f){
  char caracter;
  string trozo;
  bool salir = false;

  while (!f.eof() and !salir){
    caracter = f.get();
    cadena += caracter;
    trozo += caracter;
    if (caracter == '{'){
      RecursivoLlaves(trozo,f);
      cadena += trozo;
    }
    else if (caracter == '}'){
      salir = true;
    }
  }
  //cout << "{" << trozo << endl;
  //Pausa();
}


void RecursivoCorchetes(string &cadena, ifstream &f){
  char caracter;
  bool salir = false;

  while (!f.eof() and !salir){
    caracter = f.get();
    cadena += caracter;
    if (caracter == '['){
      RecursivoCorchetes(cadena,f);
    }
    else if (caracter == ']'){
      salir = true;
    }
  }
}


void RecursivoCadena(char caracter_fin, string &cadena, ifstream &f, int deep){
  /*for (int i=0; i<deep; i++)
    cout << " ";
  cout << "R(" << caracter_fin << ")\n";*/
  char caracter;
  bool Recursivo = true;

  if (!f.eof()){
    caracter=f.get();
    while (!f.eof() and caracter != '"' and caracter != '[' and caracter != '>' and caracter != '<' and caracter != '{' and caracter != '(' and caracter != '!'){
      cadena += caracter;
      caracter=f.get();
    }

    if (caracter!=caracter_fin){
      cadena+=caracter;
      //cout << "cadena: " << cadena << endl;
      switch(caracter){
        case '"':
          //cout << "Dobles Comillas\n";
          RecursivoCadena('"', cadena, f, deep+1);
          //cout << cadena << endl;
          //Pausa();
          cadena+='"';
        break;
        case '{':
          //cout << "Llaves\n";
          RecursivoLlaves(cadena, f);
          //cout << cadena << endl;
          //Pausa();
          //cadena+='}';
        break;
        case '[':
          //cout << "Corchetes\n";
          RecursivoCorchetes(cadena, f);
          //cout << cadena << endl;
          //Pausa();
          //cadena+=']';
        break;
        case '(':
          //cout << "Parentesis\n";
          RecursivoParentesis(cadena, f);
          //cout << cadena << endl;
          //Pausa();
          //cadena+=')';
          /*case '(': while (cadena[cadena.length()-1]!=')'){
            cadena+=f.get();
          }*/
        break;
        case '!':
          if (cadena.length()>1 and cadena.substr(cadena.length()-2,2) == "<!"){
            //cout << "Posible Comentario\n";
            caracter = f.get();
            if (caracter == '-'){
              cadena += caracter;
              //cout << "Encontrado un <!-" << endl;
              //cout<<"antes del -3"<<endl;
              while (cadena.substr(cadena.length()-3,3)!="-->"){
                cadena+=f.get();
              }
              //cout<<"despues del while"<<endl;
              //cout << cadena << endl;
              //Pausa();
              Recursivo = false;
              cadena = cadena.substr(0,cadena.length()-1);
              //f.putback('>');
              //cout << cadena << endl;
              //Pausa();
            }
            else {
              f.putback(caracter);
            }
          }
        break;
        /*case '<':
          //cout << "Angulos\n";
          RecursivoCadena('>', cadena, f, deep+1);
          //cout << cadena << endl;
          //Pausa();
          cadena+='>';
        break;*/
      }
      if (Recursivo)
        RecursivoCadena(caracter_fin, cadena, f, deep+1);
    }
  }

}


void QuitarEspacios(string &cadena){
  while (cadena[cadena.length()-1]>0 and cadena[cadena.length()-1]<=' ')
    cadena=cadena.substr(0,cadena.length()-1);

  while (cadena.length()>1 and cadena[0]>0 and cadena[0]<=' ')
    cadena=cadena.substr(1,cadena.length()-1);
}


struct estruct_tag{
  string code;
  list <string> secuencia;
};


void Descomponer_Tag(string tag, map<string,string> & lit, map<string,string> & lit_basicos, estruct_tag & contenido){
  string aux;
  int ini=1;
  char last_char, ch;
  pair <string, string> dat;
  map<string,string>::iterator it;
  estruct_tag tdescomp;

  contenido.code = tag;
  contenido.secuencia.clear();

  //cout << "tag: " << tag << endl;
  do {
    int tama=0;
    if (tag[ini+tama]=='"'){
      tama++;
      while (tag[ini+tama]!='"'){
	       tama++;
      }
    }
    else if(tag[ini+tama]=='\''){
      tama++;
      while (tag[ini+tama]!='\'' && tag[ini+tama]!='>'){
	     tama++;
      }
    }
    else if(tag[ini+tama]=='{'){
      tama++;
      while (tag[ini+tama]!='}')
	      tama++;
    }
    else if(tag[ini+tama]=='['){
      tama++;
      while (tag[ini+tama]!=']'  and tag[ini+tama]!='>')
	     tama++;
    }
    else {
      while (tag[ini+tama]!=' ' and tag[ini+tama]!='>' and tag[ini+tama]!='=' and tag[ini+tama]!='[')
	     tama++;
    }


    last_char=tag[ini+tama];
    //cout << "last_char: " << last_char << endl;

    if (last_char==' ' or last_char=='>' or last_char=='['){
      //cout << "Extraido: " << tag.substr(ini,tama) << endl;
      aux = tag.substr(ini,tama);
    }
    else {
      //cout << "Extraido: " << tag.substr(ini,tama+1) << endl;
      aux = tag.substr(ini,tama+1);
    }
    //cout << "lit: " << aux << endl;
    //cin >> ch;

    if (aux[0]=='"' and aux[aux.length()-1]=='"'){
      aux = aux.substr(1,aux.length()-2);
      QuitarEspacios(aux);
      aux = "\""+aux+"\"";
    }


    it = lit_basicos.find(aux);
    if (it!=lit_basicos.end()){
      dat.second=it->second;
    }
    else {
      it = lit.find(aux);
      if (it==lit.end()){
	dat.second="l"+to_string(lit.size());
	dat.first=aux;
	lit.insert(dat);
	//cout << "lit: " << aux << "  code: " << dat.second << endl;
      }
      else{
	dat.second = it->second;
      }
    }

    contenido.secuencia.push_back(dat.second);

    ini= ini+tama+1;
    if (last_char=='[')
      ini--;

  }while (last_char!='>');

  /*cout << "tag: " << tag << endl;
  cout << "\t";
  for (list<string>::iterator it=contenido.secuencia.begin(); it!=contenido.secuencia.end(); it++)
    cout << *it <<"  ";
  cout << endl;

  cin >> ch;*/

  //cout << endl;

}

void Cargar_File (const char * nomfich, map<string,string> & diccionario, bool is_tag){
  ifstream f(nomfich);
  pair <string, string> dato;
  int i=0;
  while (!f.eof()){
    f>> dato.first;
    if (is_tag)
      dato.second = "T"+to_string(i);
    else
      dato.second = "L"+to_string(i);

    i++;
    diccionario.insert(dato);
    //cout << dato.second << " | " << dato.first << endl;
  }
  f.close();

}

void Salvar_FileIS (const char * nomfich, map<int,string> & diccionario){
  ofstream f(nomfich);
  pair <string, string> dato;
  for (auto it=diccionario.begin(); it!=diccionario.end(); it++){
    //cout << it->first << " | " << it->second << endl;
    f << it->first << " | " << it->second << endl;
  }
  f.close();
}

void Salvar_FileVS (const char * nomfich, vector<string> & diccionario){
  ofstream f(nomfich);
  for (auto i=0; i < diccionario.size(); i++){
    //cout << it->first << " | " << it->second << endl;
    f << i << " :\t" << diccionario[i] << endl;
  }
  f.close();
}


void Minusculas(string &cadena){
  for (int i=0; i<cadena.length(); i++)
    if (cadena[i]>='A' and cadena[i]<='Z')
      cadena[i]='a'+(cadena[i]-'A');
}

pagina::pagina(const char *nomfich){
  string buffer,num, aux;
  char ch;
  int i=0;
  estruct_tag e;
  map<string,string>::iterator it;
  pair<string,string> par;

  Cargar_File ("tag_basicos.txt", tag_basicos, true);

  Cargar_File ("literal_basicos.txt", literal_basicos, false);


  ifstream f(nomfich);
  bool salir=false;

  while (!f.eof() and !salir){
    i=0;
    buffer = f.get();
    while (buffer[0]>0 and buffer[0]<=' '){
      buffer = f.get();
    }

    switch(buffer[0]){
      case '>': buffer="";
      RecursivoCadena('<', buffer, f, 0);
      //cout<<"entra";
      if (buffer.length()>1){
        //cout << "literal: " << buffer.substr(0,40) << endl;
        //cout << "literal: " <<buffer << endl;
        //cout << buffer[0] << endl;
        //Pausa();
        if (buffer[0]=='"' and buffer[buffer.length()-1]=='"'){
          buffer = buffer.substr(1,buffer.length()-2);
          QuitarEspacios(buffer);
          buffer = "\""+buffer+"\"";
        }
        QuitarEspacios(buffer);
        //buffer="\""+buffer+"\"";
        fuente.push_back(buffer);
        it = literal_notag.find(buffer);
        if (it==literal_notag.end()){
          par.first = buffer;
          par.second = "n"+to_string(literal_notag.size());
          //cout << "n" << literal_notag.size()<< ": " << buffer << endl;
          literal_notag_ord.insert(pair<int,string>(literal_notag.size(),buffer));
          literal_notag.insert(par);
        }
        else{
          par.second=it->second;
        }
        codificado.push_back(par.second);
      }
      if (!f.eof())
      f.putback('<');

      break;

      case '<': RecursivoCadena('>', buffer, f, 0);
      buffer+='>';
      //cout << "\ttag: " ;
      //cout << buffer << endl;
      //Pausa();
      //cin >> ch;


      if (!f.eof())
      f.putback('>');

      aux=buffer;
      QuitarEspacios(aux);
      Minusculas(aux);

      fuente.push_back(aux);
      //cout<<aux<<endl;

      if (aux=="</html>"){
        salir=true;
        //cout << "Salir "<< endl;
      }

      if (!salir){
        if (aux[1]=='!'){
          aux="<!>";
          it=tag_basicos.find(aux);
          if (it!=tag_basicos.end()){
            par.second=it->second;
          }
        }
        else {
          string aux2;
          if (aux.find(' ')!=string::npos)
            aux2=aux.substr(0,aux.find(' '))+">";
          else{
            aux2=aux;
          }

          it=tag_basicos.find(aux2);
          if (it!=tag_basicos.end()){
            par.second=it->second;
          }
          else {
            it=otros_tag.find(aux2);
            if (it==otros_tag.end()){
              par.first = aux2;
              num=to_string(otros_tag.size());
              par.second = "t"+num;
              otros_tag.insert(par);
            }
            else {
              par.first = aux;
              par.second=it->second;
            }
          }
          Descomponer_Tag(aux,literal_tag, literal_basicos, e);
          list<string>::iterator i=e.secuencia.begin();

          for (i++; i!=e.secuencia.end(); i++){
            par.second = par.second+"_"+*i;
          }
        }
      }
      codificado.push_back(par.second);
      break;
    }
  }

  Salvar_FileIS("literal_notag.txt", literal_notag_ord);

  for (map<string,string>::iterator it=literal_tag.begin(); it!=literal_tag.end(); it++){
    string aux=it->second;
    //cout << aux << endl;
    aux=aux.substr(1,aux.length());
    int a=atoi(aux.c_str());
    literal_tag_ord.insert(pair<int,string>(a,it->first));
  }

  Salvar_FileIS("literal_tag.txt", literal_tag_ord);
  Salvar_FileVS("codificado.txt", codificado);
  Salvar_FileVS("fuente.txt", fuente);

}

void DescomponerSequence(string secuencia, vector<string> &v){
  string aux=secuencia, aux2, tipo="T";
  size_t posT, posl, posn, menor;
  char ch;

  //cout << "Sequence: " << secuencia << endl;

  tipo=aux[0];
  aux=aux.substr(1,aux.length());
  posT = aux.find('T');
  posl = aux.find('l');
  posn = aux.find('n');
  menor= min(posT,min(posl, posn));
  while (menor!=string::npos){
    if (tipo=="T"){
      aux2 = tipo+aux.substr(0,menor);
    }
    else if (tipo=="l"){
      aux2 = "l?";
    }
    else {
      aux2 = "n?";
    }

    aux = aux.substr(menor,aux.length());

    //cout << aux2 << endl;
    //cout << aux << endl;

    v.push_back(aux2);

    tipo=aux[0];

    aux=aux.substr(1,aux.length());
    posT = aux.find('T');
    posl = aux.find('l');
    posn = aux.find('n');
    menor= min(posT,min(posl, posn));
  }

  //cout << "Salgo del ciclo\n";

  if (tipo=="T")
    v.push_back("T"+aux);
  else if (tipo=="l")
    v.push_back("l?");
  else
    v.push_back("n?");

//   cout << "Sequence: " << secuencia << endl;
//   for (int i=0; i<v.size() ; i++)
//     cout << " " << v[i];
//   cout << endl;
//   cin >> ch;
}


string Sacar_Primer_Tag(string cadena){
  if (cadena[0]=='n')
    if (cadena!="n0")
      return "n?";
    else
      return "n0";
  else if (cadena[0]=='l')
    return "l?";
  else {
    int pos=cadena.find('_');
    if (pos==string::npos)
      return cadena;
    else{
      string aux=cadena.substr(0,pos);
      return aux;
    }
  }

}

int ContarTags(string cadena){
  int cont=0;
  for (int i=0; i<cadena.length(); i++)
    if (cadena[i]=='T' or cadena[i]=='l' or cadena[i]=='n')
      cont++;
  return cont;
}


string pagina::Sacar_Opcion_Tag(string opcion, int linea){
  string aux= codificado[linea];
  size_t pos;
  map<int,string>::iterator it;
  int lit;
  pos=aux.find(opcion);
  if (pos==string::npos)
    return "";
  else {
    aux = aux.substr(pos+opcion.length(),aux.length());
    pos = aux.find('_');
    aux= aux.substr(1,pos);
    lit =atoi(aux.c_str());
    //cout << "lit: " << lit << endl;
    it =literal_tag_ord.find(lit);
    if (it!=literal_tag_ord.end()){
      return it->second;
    }
    else
      return "";
  }
}


string pagina::Sacar_Value(int linea){
  string tag = Sacar_Primer_Tag(codificado[linea]);
  if (tag=="T8")
    return Sacar_Opcion_Tag("L2_",linea);
  else if (tag=="T1")
    return Sacar_Opcion_Tag("L19_",linea);
  else if (tag[0]=='n'){
    tag = codificado[linea];
    tag=tag.substr(1,tag.length());
    int valor=atoi(tag.c_str());
    return (literal_notag_ord.find(valor))->second;
  }
  else
    return "";
}


int pagina::Buscar_Patron(string patron, int &a_partir_de, vector<string> & value){
  vector<string> v, w;
  bool encontrado=false;
  char ch;
  int tama=ContarTags(patron);
  string tag_prov, acumulado, acumulado_completo;

  value.clear();


  DescomponerSequence(patron, v);


  int linea=a_partir_de, linea_previa=a_partir_de+1;



  while (!encontrado and linea+v.size()<codificado.size()){

    tag_prov = Sacar_Primer_Tag(codificado[linea]);


    while (tag_prov!=v[0] and linea+v.size()<codificado.size()){
      linea++;
      tag_prov = Sacar_Primer_Tag(codificado[linea]);
    }
    linea_previa=linea+1;

    value.push_back( Sacar_Value(linea));

    acumulado=tag_prov;
    acumulado_completo=acumulado;
    int i=1,j=1;
    tag_prov = Sacar_Primer_Tag(codificado[linea+j]);
    while (tag_prov=="n0"){
      acumulado_completo+=tag_prov;
      j++;
      tag_prov = Sacar_Primer_Tag(codificado[linea+j]);
    }

    while (i < v.size() and tag_prov==v[i]){
      acumulado+=tag_prov;
      acumulado_completo+=tag_prov;
      value.push_back( Sacar_Value(linea+j));

      //cout << tag_prov << "  " << v[i] << endl;
      i++;
      j++;
      if (i< v.size()){
	tag_prov = Sacar_Primer_Tag(codificado[linea+j]);
	while (tag_prov=="n0"){
	  acumulado_completo+=tag_prov;
	  j++;
	  tag_prov = Sacar_Primer_Tag(codificado[linea+j]);
	}
      }
    }


    //cout << "Acumulado: " << acumulado << "   Acumulado_Completo: " << acumulado_completo <<endl;


    if (i==v.size()){
      encontrado=true;
      a_partir_de=linea+j;
      //linea=linea_previa-1;
    }
    else {
      value.clear();
      linea=linea_previa;
      linea_previa++;
    }
  }

  if (encontrado){
    return linea;
  }
  else
    return -1; // No se ha encontrado la secuencia

}


void InstanciasPatron(string patron, int pag_ini, vector<string> &instancias) {
  vector <string> v;


}

//Método para mostrar por pantalla el fuente
void pagina::Lista_Fuente(int inicio, int fin){
  for (int i=inicio; i< fin; i++){
    cout << i << ":\t | " << fuente[i] << endl;
  }
}

//Método para pasar el fuente a un string
string pagina::Lista_Fuente_String(int inicio, int fin){
  string aux;
  for (int i=inicio; i< fin; i++){
    aux+=fuente[i]+"\n";
  }
  return aux;
}

//Método para mostrar por pantalla el codificado
void pagina::Lista_Codificado(){
  for (int i=0; i< codificado.size(); i++){
    cout << i << ":\t | " << codificado[i] << endl;
  }
}

//Método para mostrar por pantalla el tag basico
void pagina::Lista_Tag_Basico(){
  for (map<string,string>::iterator it=tag_basicos.begin(); it!=tag_basicos.end(); it++){
    cout << it->second << "\t | " << it->first << endl;
  }
}

//Método para mostrar por pantalla los otros tag
void pagina::Lista_Otros_Tag(){
  for (map<string,string>::iterator it=otros_tag.begin(); it!=otros_tag.end(); it++){
    cout << it->second << "\t | " << it->first << endl;
  }
}

//Método para mostrar por pantalla el literal_basico
void pagina::Lista_Literal_Basico(){
  for (map<string,string>::iterator it=literal_basicos.begin(); it!=literal_basicos.end(); it++){
    cout << it->second << "\t | " << it->first << endl;
  }
}

//Método para mostrar por pantalla el literal_notag
void pagina::Lista_Literal_Notag(){
  for (map<string,string>::iterator it=literal_notag.begin(); it!=literal_notag.end(); it++){
    cout << it->second << "\t | " << it->first << endl;
  }
}

//Método para mostrar por pantalla el literal_tag
void pagina::Lista_Literal_Tag(){
  for (map<string,string>::iterator it=literal_tag.begin(); it!=literal_tag.end(); it++){
    cout << it->second << "\t | " << it->first << endl;
  }
}

int NumeroBarras(string cadena){
  int n=0, pos;
  pos = cadena.find("/",0);
  while (pos!=string::npos){
    n++;
    pos = cadena.find("/", pos+1);
  }
  return n;
}

int TipoArchivo(string cadena){
  //cout << "Tipo de archivo de " << cadena << endl;
  if (cadena[cadena.length()-1]=='/'){
    // es una pagina web
    return 0;
  }
  else if (NumeroBarras(cadena)<=2){
    // es una pagina web
    return 0;
  }
  else {
    cadena = cadena.substr(cadena.rfind("/")+1, cadena.length()-1);
    string ext;
    int pos2 = cadena.rfind(".");
    if (pos2 == string::npos)
      ext ="";
    else {
      ext = cadena.substr(pos2+1, cadena.length()-1);//saca la extension
      /*if ( (ext.length()>1 and ext.substr(0,2)!="js") or (ext.length()>2 and ext.substr(0,3)!="css") ){
        ext="";
      }*/
    }

    if (ext == "" or ext == "shtml" or ext == "html" or ext == "htm" or ext =="php" or ext == "xhtml"){
      // es una pagina web
      //cout << "Extension: " << ext << endl;
      return 0;
    }
    else if (ext == "jpeg" or ext == "jpg" or ext=="gif" or ext == "png" or ext == "bmp"){
      // es una imagen
      //cout << "Extension: " << ext << endl;
      return 1;
    }
    else if (ext == "avi" or ext == "mpeg" or ext == "mpg" or ext == "flv" or ext == "fid" or ext == "wmv" or ext == "mp4" or ext == "mov"){
      // es un video
      //cout << "Extension: " << ext << endl;
      return 2;
    }
    else if (ext == "torrent"){
      // enlace torrent
      //cout << "Extension: " << ext << endl;
      return 4;
    }
    else if (ext == "pdf"){
      // enlace pdf
      //cout << "Extension: " << ext << endl;
      return 5;
    }
    else if (ext == "ppt" or ext == "pptx"){
      // enlace powerpoint
      //cout << "Extension: " << ext << endl;
      return 6;
    }
    else {
      // otro tipo de archivo
      return 3;
    }
  }
}

int Numero_Palabras(string frase){
  int num_palabras;
  char oracion[100];
  int i, contPalabras = 1;

  for(i = 0; i < frase.size(); i++){
    if(frase[i] == ' ' && i > 0 && frase[i - 1] != ' '){
      contPalabras++;
    }
  }
  //printf("Cantidad de palabras: %d\n", contPalabras);
  return contPalabras;
}

int Numero_Caracteres(string frase){
  return frase.size();
}

bool PrimerCaracterBlanco(string frase){
  int i;
  bool espacios=false;
  for(i = 0; i < frase.size(); i++){
      if(frase[i] == ' ' && frase[i + 1] == ' '){
        espacios=true;
      }
  }
  return espacios;
}

//Carga los datos de un fichero en una lista. Para comprobar si las palabras son irrelevantes.
void Cargar_Palabras_Irrelevantes(const char *nomfich, list<string> &datos){
  //cout << "Cargando  palabras irrelevantes...\n";
  ifstream f(nomfich);
  string aux;
  set<string> lista;
  int i=1;

  while (!f.eof()){
    if (i%1000==0)
      cout << i << endl;
    getline(f, aux);
    if (lista.find(aux)==lista.end() && aux!=""){
      datos.push_back(aux);
      lista.insert(aux);
    }
    i++;
  }

  f.close();
}

//Método para filtrar las palabras clave que se van a usar para decidir si una pagina es relevante.
void Filtrar_Keys( map<string,int> &palabras_c){
  int num_palabras;
  bool borrado;
  list<string> palabras_irrelevantes;
  map<string,int>::iterator it=palabras_c.begin();
  Cargar_Palabras_Irrelevantes("palabras_irrelevantes.txt", palabras_irrelevantes);
  /*for (auto i = palabras_irrelevantes.begin(); i!= palabras_irrelevantes.end(); i++){
    cout << *i << endl;
  }
  Pausa();*/
  while(it!=palabras_c.end()){
    borrado=false;
    num_palabras=Numero_Palabras(it->first);

    //cout<<"Filtrando la palabra:-->"<<it->first<<"<--N.P="<<num_palabras<<" N.C.="<<Numero_Caracteres(it->first)<<endl;

    if(num_palabras >= 4){
      //cout<<"mas de 4 palabras"<<endl;
      it = palabras_c.erase(it);
    }
    else if(Numero_Caracteres(it->first)<=2){
      //cout<<"menos de 2 caracteres"<<endl;
      it = palabras_c.erase(it);
    }
    else if(Numero_Caracteres(it->first)>=50){
      //cout<<"mas de 70 caracteres"<<endl;
      it = palabras_c.erase(it);
    }
    else if(PrimerCaracterBlanco(it->first)){
      //cout<<"mas de 2 espacios en blanco"<<endl;
      it = palabras_c.erase(it);
    }
    else if(it->first.find("[0-9]+")!=-1){
      //cout<<"contiene un numero"<<endl;
      it = palabras_c.erase(it);
    }
    else if(it->first.find(",")!=-1 || it->first.find(".")!=-1 || it->first.find("|")!=-1 || it->first.find("}")!=-1 || it->first.find(":")!=-1 || it->first.find('\n')!=-1){
      //cout<<"contiene un , . {} | : /n"<<endl;
      it = palabras_c.erase(it);
    }
    else{
      for (auto t = palabras_irrelevantes.begin(); t!= palabras_irrelevantes.end(); t++){
        if(it->first.find(*t)!=-1){
          //cout<<"Se borra esta dentro del for "<<*t<<endl;
          it = palabras_c.erase(it);
          borrado=true;
          break;
        }
      }
      //cout << "antes de entrar al if de borrado" <<borrado<< '\n';
      if(borrado!=true){
        it++;
      }
    }
  }
}

//Metodo para darle preferencia a los nodos cuya url tienen palabras que nos interesan
bool Preferente(string cadena){
  if (cadena.find("Artificial Intelligence")!=string::npos){
    return true;
  }
  else if (cadena.find("applet")!=string::npos){
    return true;
  }
  else if (cadena.find("Neural Networks")!=string::npos){
    return true;
  }
  else if (cadena.find("applets")!=string::npos){
    return true;
  }
  else if (cadena.find("Search")!=string::npos){
    return true;
  }
  else if (cadena.find("aispace")!=string::npos){
    return true;
  }
  return false;
}

//Carga los datos de un fichero en una lista. Para comprobar si en la url hay palabras irrelevantes.
void Cargar_PalabrasWeb_Irrelevantes(const char *nomfich, list<string> &datos){
  //cout << "Cargando  palabras irrelevantes...\n";
  ifstream f(nomfich);
  string aux;
  set<string> lista;
  int i=1;

  while (!f.eof()){
    if (i%1000==0)
      cout << i << endl;
    getline(f, aux);
    if (lista.find(aux)==lista.end() && aux!=""){
      datos.push_back(aux);
      lista.insert(aux);
    }
    i++;
  }

  f.close();
}

//Comprueba si un string es irrelevante o no(cargando las palabras de un txt)
bool Irrelevante(string cadena){
  bool contiene=false;
  list<string> palabrasweb_irrelevantes;
  Cargar_PalabrasWeb_Irrelevantes("palabrasweb_irrelevantes.txt", palabrasweb_irrelevantes);
  for (auto pi = palabrasweb_irrelevantes.begin(); pi!= palabrasweb_irrelevantes.end(); pi++){
    if(cadena.find(*pi)!=-1){
      //cout<<cadena<<" Contiene una palabra irrelevante en la url --> "<<*pi<<endl;
      contiene = true;
      break;
    }
  }
  if (contiene){
    return true;
  }
  else{
    return false;
  }
}

//Comprueba si una imagen es irrelevante o no(cargando las palabras de un txt)
bool Imagen_Irrelevante(string cadena){
  bool contiene=false;
  list<string> imagenes_irrelevantes;
  Cargar_PalabrasWeb_Irrelevantes("imagenes_irrelevantes.txt", imagenes_irrelevantes);
  for (auto pi = imagenes_irrelevantes.begin(); pi!= imagenes_irrelevantes.end(); pi++){
    if(cadena.find(*pi)!=-1){
      //cout<<cadena<<" Contiene una palabra irrelevante en la url --> "<<*pi<<endl;
      contiene = true;
      break;
    }
  }
  if (contiene){
    return true;
  }
  else{
    return false;
  }
}

//Devuelve el maximo valor encontrado en el second del map
int max_map_value(const map<string, int> &m){
    map<string, int>::const_iterator it = m.begin();
    int highest = it->second;
    for(map<string, int>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        if(it->second > highest)
            highest = it->second;
    }
    return highest;
}

//Método para pasar un string a minúsculaS
void tolower(string &palabra){
  for(int i=0; i<palabra.size(); i++){
    palabra[i]=tolower(palabra[i]);
  }
}

//Comprueba si la palabra tiene una s al final o no:
//Si la tiene se la quita y si no la tiene se la añade
void add_s(string &palabra){
  int tamanio=palabra.size()-1;
  string ultimo_caracter=palabra.substr(tamanio);
  if(ultimo_caracter == "s"){
    //cout<<"termina en s, borrandola para comprobar el singular"<<endl;
    palabra.erase(tamanio);
  }
  else{
    //cout<<"NO termina en s, añadiendola para comprobar el plural"<<endl;
    palabra.push_back('s');
  }

}

//Carga la lista de sinonimos guardada en sinonimos.txt en una lista
void Cargar_Sinonimos(const char *nomfich, list<string> &datos){
  //cout << "Cargando  palabras irrelevantes...\n";
  ifstream f(nomfich);
  string aux;
  set<string> lista;
  int i=1;

  while (!f.eof()){
    if (i%1000==0)
      cout << i << endl;
    getline(f, aux);
    if (lista.find(aux)==lista.end() && aux!=""){
      datos.push_back(aux);
      lista.insert(aux);
    }
    i++;
  }

  f.close();
}

//Carga la información del fichero en la estructura set.
void Cargar_Salida_Ontologia_InSet(const char *nomfich, set<string> & datos){
  ifstream f (nomfich);
  string aux;

  while (!f.eof()){
    f >> aux;
    datos.insert(aux);
  }

  f.close();
}

//Busca en la Ontologia "palabra" para comprobar si tiene sentido
bool Buscar_en_Ontologia(string palabra){
  bool encontrada=false;
  set<string> set_ontologia;
  string comando = "./owltools wiki_ai_sa.obo --ancestors ";
  string word, llamada;
  string final = " > resultado_busqueda_ontologia.txt 2> /dev/null";
  llamada=comando+ "\""+palabra+"\" "+ final;
  system(llamada.c_str());//ejecuta la llamada
  Cargar_Salida_Ontologia_InSet("resultado_busqueda_ontologia.txt", set_ontologia);

  for (auto os = set_ontologia.begin(); os!= set_ontologia.end(); os++){
    //cout << *os << endl;
    string aux_o=*os;
    if(aux_o.find("IDENTITY") != -1){
      encontrada=true;
      break;
    }
  }
  return encontrada;
}

void pagina::Sacar_http(list<string> &cola, set<string> &images, bool &contenido_relevante, map<string,int> &palabras_clave, string &palabra_clave_encontrada){//guarda las paginas en cola
/*
  string hola = "hola";
  cout<<hola<<endl;
  add_s(hola);
  cout<<hola<<endl;
  Pausa();
*/
  /*string comando = "./owltools ~/Escritorio/wiki_ai_l6.obo --ancestors ";
  string word, llamada;
  string final = " > resultado_busqueda_ontologia.txt";
  llamada=comando+ "\""+word+"\" "+ final;
  set<string> set_ontologia;*/
  set<string>::iterator it_ontologia;
  bool encontrada_en_ontologia=false;
  string word;

  list<string> hijos;
  set<string> imagenes;
  //Lista_Literal_Tag();
  for (map<string,string>::iterator it=literal_tag.begin(); it!=literal_tag.end(); it++){
    //cout<<it->first<<endl;
    if (it->first.size()>5 and it->first.substr(1,4) == "http"){
      string aux = it->first.substr(1, it->first.length());//coje todo el enlace
      aux = aux.substr(0,aux.length()-1);
      //cout << "preguntando tipo\n";
      int tipo = TipoArchivo(aux);
      //cout << "captado tipo\n";
      switch (tipo) {
        case 0: hijos.push_back(aux);//es una pagina web y la guarda en la cola de hijos
                break;
        case 1: imagenes.insert(aux);
                break;
        /*
        case 2: torrentes.insert(aux);
                break;
        case 3: pdf.insert(aux);
                break;
        case 4: ppt.insert(aux);
                break; */
      }
    }
  }

  for (auto it = hijos.begin(); it != hijos.end(); it++){
    if (Preferente(*it)){
      cola.push_front(*it);
    }
    else if (Irrelevante(*it) == false){
      //cout <<"añadido como no Irrelevante  "<< *it<<endl;
      cola.push_back(*it);
    }
  }


/*
  for (map<string,string>::iterator it=literal_tag.begin(); it!=literal_tag.end(); it++){
    cout << it->first << endl;
  }*/
/*
  for (map<string,int>::iterator it=palabras_clave.begin(); it!=palabras_clave.end(); it++){
    cout << it->first <<"--"<<it->second << endl;
  }*/

  //1- ACTUALIZAMOS LAS PALABRAS CLAVE EXPLORANDO LAS PALABRAS DE NOTAG PARA VER CUAL SE REPITE MÁS

  pair<string,int> par_c;
  map<string,int>::iterator p;
  par_c.second = 1;
  for (map<string,string>::iterator it=literal_notag.begin(); it!=literal_notag.end(); it++){
    par_c.first = it->first;
    p = palabras_clave.find(it->first);//no entra dentro del first, lo compara entero
    if (p!=palabras_clave.end()){
      p->second = p->second+1;
    }
    else{
      palabras_clave.insert(par_c);
    }
  }

  //2- FILTRAR PALABRAS CLAVE
  Filtrar_Keys(palabras_clave);
/*
  cout<<endl;
  cout << "Número de palabras claves despues de filtrar NOTAG: "<< palabras_clave.size()<<endl;
  for (map<string,int>::iterator it=palabras_clave.begin(); it!=palabras_clave.end(); it++){
    cout << it->first <<"====>>"<<it->second << endl;
    //cout << "tamaño: "<< palabras_clave.size()<<endl;
  }
*/


  //3- ORDENAR DESCENDENTEMENTE Y OBTENER 50 PALABRAS QUE MÁS APARECEN
  list <string> palabras_clave_ordenadas;
  int maxpos=max_map_value(palabras_clave);//obtenemos el mayor valor de los repetidos
  cout<<"Elemento mas veces repetido: "<<maxpos<<endl;

  for(int i = maxpos; i>=1; i--){
    for(map<string, int>::const_iterator ut = palabras_clave.begin(); ut != palabras_clave.end(); ++ut){

          if(ut->second == i){
              //cout << it->first << "\t" << it->second << endl;
              if(palabras_clave_ordenadas.size()<=40){
                palabras_clave_ordenadas.push_back(ut->first);//guardamos en una lista de string las palabras
              }
          }
      }
  }


  cout<<"------------------------ PALABRAS CLAVE-------------"<<endl;
  for (list<string>::iterator it=palabras_clave_ordenadas.begin(); it!=palabras_clave_ordenadas.end(); it++)
    cout << *it <<endl;
  cout<<"------------------------ PALABRAS CLAVE-------------"<<endl;



  //4- COMPROBAMOS QUE HAY ALGUNA DE LAS PALABRAS CLAVE PARA AÑADIR A LA PAGINA
  contenido_relevante=false;
  palabra_clave_encontrada="ninguna";
  cout<<"---->Tamaño literal:notag:::<----"<<literal_notag.size()<<endl;
  for (map<string,string>::iterator it=literal_notag.begin(); it!=literal_notag.end(); it++){
    bool encontrado=false;
    //cout<<"SIGUIENTE ITERACION"<<endl;
    if (it->first.find("applet") != -1){
      contenido_relevante=true;
      palabra_clave_encontrada="applet";
      cout<<"palabra Applet encontrada"<<endl;
      //cout<<"entra en el IF del applet"<<endl;
      break;
    }
    else{
      list <string>::iterator iter;
      //cout<<"entra en el ELSE"<<endl;
      for (list<string>::iterator pco=palabras_clave_ordenadas.begin(); pco!=palabras_clave_ordenadas.end(); pco++){
        if(it->first.find(*pco) != -1){
          //palabra_clave_encontrada=*pco;
          cout<<"palabra clave encontrada= "<<*pco<<endl;
          cout<<"Comprobando en la ontologia..."<<endl;

          //COMPROBAMOS EN LA ONTOLOGIA ANTES DE ACEPTARLA
          word=*pco;
          tolower(word);
          encontrada_en_ontologia=Buscar_en_Ontologia(word);
          //----------------------------------------------------------------------------------------------

          //1-Buscamos en la Ontologia la palabra original en minuscula para que la acepte
          if(encontrada_en_ontologia){
            cout<<"Palabra original encontrada en la Ontologia: "<<" "<<word<<endl;
          }

          //2- Comprobamos si la ultima letra es una s, sino la añadimos y volvemos a buscar en la ontologia, si es se la quitamos y buscamos
          if(!encontrada_en_ontologia){
              string cs_s;
              cs_s=word;
              add_s(cs_s);
              encontrada_en_ontologia=Buscar_en_Ontologia(cs_s);
              if(encontrada_en_ontologia){
                cout<<"Palabra modificada encontrada en la Ontologia: "<<" "<<cs_s<<endl;
              }
          }

          //3- Buscamos en la lista de sinonimos
          //word="neural";
          if(!encontrada_en_ontologia){
            list<string> sinonimos;
            int t=0;
            list<string>::iterator sino;
            string sinonimo_a_buscar;
            Cargar_Sinonimos("sinonimos.txt",sinonimos);
            for (sino=sinonimos.begin(); sino!=sinonimos.end(); sino++){
              string aux_sin=*sino;
              if(aux_sin.find(word) != -1){
                cout<<"Encontrado en la lista de sinonimos...Buscando sinonimo en la Ontologia"<<endl;
                cout<<aux_sin<<endl;
                //Nos quedamos con el sinonimo que esta despues del caracter ->
                for(int i=0; i<aux_sin.size();i++){
                  t++;
                  if(aux_sin[i]=='-' && aux_sin[i+1]=='>'){
                    //cout<<aux_sin[i+1]<<endl;
                    sinonimo_a_buscar=aux_sin.substr(t+1,aux_sin.length());
                    //cout<<sinonimo_a_buscar<<endl;
                  }
                }
                //BUSCAMOS EL SINONIMO EN LA ONTOLOGIAs
                encontrada_en_ontologia=Buscar_en_Ontologia(sinonimo_a_buscar);
                break;
              }
            }
            if(encontrada_en_ontologia){
              cout<<"Sinonimo encontrado en la Ontologia: "<<" "<<sinonimo_a_buscar<<endl;
            }
          }

          //4- Buscamos en la lista de acronimos
          //word="neural";
          if(!encontrada_en_ontologia){
            list<string> acronimos;
            int t=0;
            list<string>::iterator sino;
            string sinonimo_a_buscar;
            Cargar_Sinonimos("acronimos.txt",acronimos);
            for (sino=acronimos.begin(); sino!=acronimos.end(); sino++){
              string aux_sin=*sino;
              if(aux_sin.find(word) != -1){
                cout<<"Encontrado en la lista de acronimos"<<endl;
                cout<<aux_sin<<endl;
                encontrada_en_ontologia=true;
                break;
              }
            }
            if(encontrada_en_ontologia){
              cout<<"Acronimo encontrado: "<<" "<<sinonimo_a_buscar<<endl;
            }
          }
          //Pausa();
          //----------------------------------------------------------------------------------------------
          if(encontrada_en_ontologia){
            //cout<<"Encontrada en la ontologia"<<endl;
            palabra_clave_encontrada=*pco;
            encontrado=true;
            //Pausa();
            //cout<<"Encontrados en cada metodo: Original-"<<original<<" Con s-"<<con_s<<" En sinonimos-"<<en_sinonimo<<endl;
            break;
          }
        }//if
      }//for

      if(encontrado){
        contenido_relevante=true;
        //cout<<"valor hay applet dentro del segundo if del else"<<contenido_relevante<<endl;
        break;
      }
    }//else
  }
  //Añadimos las imagenes encontradas si la pagina es relevante
  if(contenido_relevante){
    for (auto et = imagenes.begin(); et!= imagenes.end(); et++){
      if(!Imagen_Irrelevante(*et)){
        images.insert(*et);
        //cout<<"Añadiendo imagenes  "<<*et<<endl;
      }
    }
  }
  //Pausa();
}
