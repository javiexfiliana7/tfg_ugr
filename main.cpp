#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <queue>
#include <map>
#include <set>
#include "pagina.h"

using namespace std;

struct informacion{
    string titulo;
    string enlace;
    int repetido;
};

//Método para pausar el programa. Pulsar una tecla para continuar.
void Pause(){
    char ch;
    cout << "Pulsa una tecla ..." << endl;
    cin >> ch;
  }

//Método para comprobar si existe o no un fichero
bool Existe(const char *nomfich){
    ifstream f(nomfich);

    if (!f)
      return false;
    else
      return true;
  }

//Salvar en un fichero la informacion de cada enlace.
//(Titulo,Enlace, Nº de veces que se repite)
void Salvar_Informacion(const char * nomfich, map<string, informacion> &datos){
  ofstream f (nomfich);

  for (map<string,informacion>::iterator it=datos.begin(); it!=datos.end(); it++){
   f << it->second.titulo << endl;
   f << it->second.enlace << endl;
   f << it->second.repetido << endl;
  }

  f.close();
}

//Cargar la información de un fichero a una estructura map
void Cargar_Informacion(const char *nomfich, map<string, informacion> &datos){
  cout << "Cargando cerrados...\n";
  ifstream f(nomfich);
  informacion aux;
  bool dentro=false;
  string sub,sub_cuatro;
  /*
  f >> sub;
  sub_cuatro=sub.substr(0,4);
  if(sub_cuatro=="http"){
      dentro=true;
    }
    while(sub_cuatro != "http"){
      f >> sub;
      sub_cuatro=sub.substr(0,4);
      if(sub_cuatro=="http"){
        dentro=true;
      }
      }
  */
  while (!f.eof()){
  /*  if(dentro){
      aux.titulo=sub;
      dentro=false;
    }
    else{*/
      f >> aux.titulo;
    //}
    f >> aux.enlace;
    f >> aux.repetido;
    datos.insert(pair<string, informacion> (aux.enlace, aux));
  }
  f.close();
}

//Método para guardar las palabras clave en un fichero
void Salvar_PalabrasClave(const char * nomfich, map<string, int> &pc){
  ofstream f (nomfich);

  for (map<string,int>::iterator it=pc.begin(); it!=pc.end(); it++){
   f << it->first << endl;
   f << it->second << endl;
  }

  f.close();
}

//Método para cargar las palabras clave de un fichero
void Cargar_PalabrasClave(const char *nomfich, map<string, int> &pc){
  cout << "Cargando palabras clave...\n";
  ifstream f(nomfich);
  pair <string, int> aux;
  string aux2,aux3;
  int k;
  while (!f.eof()){
    getline(f,aux3);
    f>>k;
    getline(f,aux2);
    //k=atoi(aux2.c_str());
    if(k>1){
      //cout<<aux3<<" -- "<<k<<endl;
      pc.insert(pair<string, int> (aux3, k));
    }
  }
  f.close();
}

//Método para cargar, las palabras que no deben de aparecer
//en la url de un nodo, de un fichero a una lista
void Cargar_PalabrasWeb_Irre(const char *nomfich, list<string> &datos){
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

//Método para guardar los nodos que están abiertos(aún sin explorar) en un fichero
void Salvar_Abiertos(const char * nomfich, const list<string> &datos){
  ofstream f (nomfich);

  for (auto it = datos.begin(); it!= datos.end(); it++){
    f << *it << endl;
  }
  f.close();
}

//Método para cargar los nodos que están abiertos(aún sin explorar) de un fichero
//a una lista. También hace un filtrado para eliminar los irrelevantes.
void Cargar_Abiertos(const char *nomfich, list<string> &datos){
  cout << "Cargando  y filtrando abiertos...\n";
  ifstream f(nomfich);
  string aux;
  set<string> lista;
  int i=1;
  list<string> palabrasweb_irre;
  Cargar_PalabrasWeb_Irre("palabrasweb_irrelevantes.txt", palabrasweb_irre);


  while (!f.eof()){
    if (i%1000==0)
      cout << i << endl;
    f >> aux;
    if (lista.find(aux)==lista.end()){
      datos.push_back(aux);
      lista.insert(aux);
    }
    i++;
  }

  f.close();
  //cout<<"antes de filtrar "<<datos.size()<<endl;
  list<string>::iterator it=datos.begin();
  bool borrado;
  int n=0;
  while(it!=datos.end()){
    borrado=false;
    for (auto pi = palabrasweb_irre.begin(); pi!= palabrasweb_irre.end(); pi++){
      if((*it).find(*pi)!=string::npos){
        //cout<<"se borra: "<<*it<<" encontrado"<<*pi<<endl;
        it=datos.erase(it);
        borrado=true;
        n++;
        break;
      }
    }
    if(borrado!=true){
      it++;
    }
  }
  /*cout<<"n "<<n<<endl;
  cout<<"despues de filtrar"<<datos.size()<<endl;
  for (auto li = datos.begin(); li!= datos.end(); li++){
    cout<<*li<<endl;
  }*/
  Salvar_Abiertos("abiertos.txt", datos);
}

//Método para salvar una estructura set en un fichero
void Salvar_Set(const char * nomfich, const set<string> &datos){
  ofstream f (nomfich);

  for (auto it = datos.begin(); it!= datos.end(); it++){
    f << *it << endl;
  }
  f.close();
}

//Método para cargar una estructura set de un fichero
void Cargar_Set(const char *nomfich, set<string> & datos){
  ifstream f (nomfich);
  string aux;

  while (!f.eof()){
    f >> aux;
    datos.insert(aux);
  }

  f.close();
}

//Método para salvar una estructura map< string,list<string> > en un fichero
void Salvar_Map(const char * nomfich, const map< string,list<string> > &m){
  ofstream f (nomfich);

  for(map<string, list<string>>::const_iterator h = m.begin(); h != m.end(); ++h){
    f<<h->first<<endl;
    for (list<string>::const_iterator p=(h->second).begin(); p!=(h->second).end(); p++){
      f<< *p <<endl;
    }
  }
  f.close();
}

//Método para salvar una estructura map<string, map<string, list<string>>> en un fichero.
//Es la que vamos a usar para guardar la estructura Palabra Clave -> Dominio - Enlaces
void Salvar_MapMap(const char * nomfich, const map<string, map<string, list<string>>> &m){
  ofstream f (nomfich);

  for(map<string, map<string, list<string>>>::const_iterator h = m.begin(); h != m.end(); ++h){
    f<<h->first<<endl;
    for(map<string, list<string>>::const_iterator j = h->second.begin(); j != h->second.end(); ++j){
      f<<j->first<<endl;
      for (list<string>::const_iterator p=j->second.begin(); p!=j->second.end(); p++){
        f<< *p <<endl;
      }
    }
  }
  f.close();
}

//Método para cargar una estructura map<string, map<string, list<string>>> de un fichero.
//Es la que vamos a usar para guardar la estructura Palabra Clave -> Dominio - Enlaces
void Cargar_MapMap(const char *nomfich, map<string, map<string, list<string>>> & m){
  cout << "Cargando palabras-dominios-enlaces...\n";
  ifstream f(nomfich);
  string aux,aux2,aux3,ll,sub;
  list<string> l_aux;
  pair<string, map<string, list<string>> > mm_aux;
  pair<string, list<string>> m_aux;
  map<string, map<string, list<string>>>::iterator i_mm;
  map<string, list<string>>::iterator i_m;
  bool k=false;
  bool nuevo_dominio=false;

  while (!f.eof()){
    if(!k){
      getline(f,aux);
    }
    getline(f,aux2);
    ll=aux2.substr(0,4);
    if(ll == "http"){
      aux3=aux2;
      aux2=aux;
      aux=sub;
      nuevo_dominio=true;
    }
    else{
      getline(f,aux3);
      ll=aux3.substr(0,4);
      nuevo_dominio=false;
    }

    //cout<<"aux ="<<aux<<endl;
    //cout<<"aux2 ="<<aux2<<endl;
    //cout<<"aux3 ="<<aux3<<endl;
    //cout<<"ll ="<<ll<<endl;
    //Pause();
    while(ll=="http"){
      l_aux.push_back(aux3);
      getline(f,aux3);
      ll=aux3.substr(0,4);
      if (ll=="http") {
        //cout<<aux3<<endl;
      }
    }
    /*cout<<"-------------------"<<endl;
    for (auto it = l_aux.begin(); it!=l_aux.end(); it++){
      cout<<*it<<endl;
    }
    cout<<"-------------------"<<endl;
    Pause();*/
    if(!nuevo_dominio){
      //cout<<"if"<<endl;
      mm_aux.first=aux;
      m_aux.first=aux2;
      //m_aux.second=l_aux;
      for (auto it = l_aux.begin(); it!=l_aux.end(); it++){
        m_aux.second.push_back(*it);
      }
      mm_aux.second.insert(m_aux);
      //meter datos. aux=palabra   aux2=dominio  l_aux=lista con enlacs
      m.insert(mm_aux);
      m_aux.second.clear();
      mm_aux.second.clear();
    }
    else{
      //cout<<"else"<<endl;
      i_mm = m.find(aux);
      m_aux.first=aux2;
      m_aux.second=l_aux;
      i_mm->second.insert(m_aux);
    }
    l_aux.clear();
    k=true;
    sub=aux;
    aux=aux3;
  }

  f.close();
}


//Método para crear el body simple que se usará en el html para crear la web.
//Devuelve un string con todos los enlaces
string Body(set<string> datos){
  string aux;
  for (auto it = datos.begin(); it != datos.end(); it++){
    aux += "<a href=\"" + *it + "\"> " + *it + " </a> <br>\n";
  }

  return aux;
}

//Método para crear el body a partir de una estructura map< string,list<string> > que se usará en el html para crear la web.
//Devuelve un string con los dominios y todos los enlaces pertenecientes a cada uno
string BodyMap(const map< string,list<string> > m){
  string aux;
  for(map<string, list<string>>::const_iterator h = m.begin(); h != m.end(); ++h){
    aux += "<h1>" + h->first + " </h1> <br>\n";
    for (list<string>::const_iterator p=(h->second).begin(); p!=(h->second).end(); p++){
      aux += "<a href=\"" + *p + "\"> " + *p + " </a> <br>\n";
    }
  }

  return aux;
}

//Método para crear el body a partir de una estructura map<string, map<string, list<string>>> que se usará en el html para crear la web.
//Devuelve un string con las palabras clave, las cuales llevan asociadas los dominios y todos los enlaces pertenecientes a cada uno.
string BodyMapMap(const map<string, map<string, list<string>>> m){
  string aux,aux2,aux3;
  list<string> enl;
  int t=0;
  //<a href="destino.php?saludo=hola&texto=Esto es una variable texto">Paso variables saludo y texto a la página destino.php</a>
  aux2="indices_paginas.php";
  /*aux += "<?php\n";
  aux += "error_reporting(E_ALL);\n";
  aux += "ini_set('display_errors', '1');\n";
  aux += "session_start();\n";
  aux += "$servername='localhost';\n";
  aux += "$dbusername='root';\n";
  aux += "$dbpassword='pujillo7';\n";
  aux += "$dbname='tfg';\n";
  aux += "$conexion=mysqli_connect(\"$servername\",\"$dbusername\",\"$dbpassword\");\n";
  aux += "mysqli_select_db($conexion,\"$dbname\");\n";

  aux += "?>\n";*/
  //
  for(map<string, map<string, list<string>>>::const_iterator a = m.begin(); a != m.end(); ++a){
    aux += "<h1>" + a->first + " </h1> <br>\n";
    for(map<string, list<string>>::const_iterator h = a->second.begin(); h != a->second.end(); ++h){
      for (list<string>::const_iterator p=(h->second).begin(); p!=(h->second).end(); p++){
        aux3 += *p + "<br>";
      }

      aux2="indices_paginas.php?m1=" + a->first + "&m2=" + h->first + "&m3=" + aux3;
      aux += "<a href=\"" + aux2 + "\"> " + h->first + " </a> <br>\n";
      aux3="";
    }
    //cout<<aux;
  }

  return aux;
}

//Método que se usará para crear la galeria de imagenes que se muestra en la web.
string BodyI(set<string> datos){
  string aux;
  for (auto it = datos.begin(); it != datos.end(); it++){
    //aux += "<a> <img src=\"" + *it + "\"> </a> <br>\n";
    aux += "<img class=\"miniatura\" onclick=\"javascript:document.getElementById('imgGaleria').src=this.src;\" src=\"" + *it + "\"/>\n";

  }

  return aux;
}

//Método que crea la página web resultante con los resultados obtenidos.
void Crear_html(const char *nomfich, string base, string cadena, string galeria){
  ofstream f (nomfich);
  string aux,stylesheet,style,text,al;
  stylesheet="stylesheet";
  style="./style.css";
  text="text/css";
  al="all";

  f << "<html>\n";

  f << "<head>\n";
    f << "<link rel=\""+stylesheet+"\" href=\""+style+"\" type=\""+text+"\" media=\""+al+"\" />"<<endl;
    f << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" crossorigin=\"anonymous\">"<<endl;
    f << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap-theme.min.css\" crossorigin=\"anonymous\">"<<endl;
    f << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\" crossorigin=\"anonymous\"></script>"<<endl;
    f << "<base href=\""+ base + "\"/>\n";
    f << "<h2 id=\"titulo\">" + base + " </h2> <br>\n";
    f << "<title> Informacion </title>\n";
  f << "</head>\n";


  f << "<body>\n";
    f << "<div id=\"contenedor\">\n"<<endl;
    f << "<div id=\"enlaces\">" + cadena + "</div>" << endl;
    f << "<div id=\"barra_lateral\">\n";
    f << "<a href=\"https://www.ugr.es/\"><img src=\"logo_ugr.png\" alt=\"ugr\" /></a>" << endl;
    f << "<a href=\"http://etsiit.ugr.es/\"><img src=\"logo_etsiit.png\" alt=\"ETSIIT\" /></a>" << endl;
    f << "<a href=\"https://github.com/javiexfiliana7\"><img src=\"github.png\" alt=\"GitHub\" /></a>" << endl;
    f << "<div> <iframe src=\"https://www.zeitverschiebung.net/clock-widget-iframe-v2?language=es&timezone=Europe%2FMadrid\" width=\"100%\" height=\"150\" frameborder=\"0\" seamless></iframe></div>" << endl;
    f << "</div>\n";
    f << "</div>\n";
    f << "<div id=\"galeria\">\n";
      f << galeria << endl;
    f << "</div>\n";
  f << "</body>\n";

  f << "</html>\n";
  f.close();
}

//Método para filtrar los nodos abiertos con los ya cerrados.
//Comrpueba si el nodo no ha sido explorado. Si ha sido Explorado lo eliminamos de los abiertos.
void Filtrar_Abiertos(list<string> & abiertos, const map<string,informacion> & cerrados ){
  cout << "\t\tFiltrando Abiertos con los ya cerrados...\n";
  set<string> lista;
  list<string> copia;
  cout << "\t\t\tTamano Abiertos: " << abiertos.size() << endl;
  for (auto it = abiertos.begin(); it!=abiertos.end(); it++){
    if (lista.find(*it) == lista.end() and cerrados.find(*it) == cerrados.end()){
      copia.push_back(*it);
      lista.insert(*it);
      //cout << "\t\t\tnodo no repetido: " << *it << endl;
    }
  }
  cout << "\t\t\tTamano Copia: " << copia.size() << endl;

  abiertos.clear();

  abiertos = copia;
}

//Método para extraer el dominio de una url
string ExtraerDominio(string url){
  string aux,url_aux,dom;
  int pos;
  if(url.substr(7,3)=="www"){//http
    url_aux=url.substr(11,url.length());
  }
  else if(url.substr(8,3)=="www"){//https
    url_aux=url.substr(12,url.length());
  }
  else if(url.substr(0,3)=="www"){//Para los que no contienen http
      url_aux=url.substr(4,url.length());
  }
  else{//Para los que no contienen www
    if(url.substr(0,5)=="https"){
      url_aux=url.substr(8,url.length());
    }
    else{
      url_aux=url.substr(7,url.length());
    }
  }
  pos = url_aux.find_first_of("/");
  if(pos != -1){
    dom=url_aux.substr(0,pos);
    cout<<"Dominio del nodo: "<<dom<<endl;
  }
  else{
    dom=url_aux;
    cout<<"Dominio del nodo(no encontrado /): "<<dom<<endl;
  }

  return dom;
}

int main(int argc, char *argv[]){

//string comando = "wget -o logfile ‐‐refer=http://google.com ‐‐execute robots=off ‐U Mozilla ";
  string comando = "wget -t 3 -o logfile ‐‐refer=http://google.com ‐‐execute robots=off ‐U Mozilla ";
  string enlace1 = "http://www.aispace.org";
  string final = " -O fichero.html";
  string llamada;
  char ch;
  bool hay_contenido;
  string dominio;
  /*string p="http://webcourse.cs.technion.ac.il/236501/Winter2007-2008/lnk.html";
  dominio=ExtraerDominio(p);
  cout<<dominio<<endl;
  Pause();*/
  //pagina page("fichero.html");



  list<string> abiertos;
  string body_imagenes, applet_html, palabra_paginas_html, palabra_clave_encontrada, nodo;
  set<string> imagenes, url_pag_relevantes;
  map<string,informacion> ya_existe;
  map<string, map<string, list<string>> > palabra_paginas;
  map<string,int> palabras_clave_usadas;
  map<string, map<string, list<string>>>::iterator iter;
  map<string, list<string>>::iterator iterdos;
  pair<string,int> par_c;
  pair<string,informacion> par;

  par_c.first="applet";//Añadimos la palabra "applet" a las palabras clave
  par_c.second=1;
  palabras_clave_usadas.insert(par_c);

  int old_n_imagenes = imagenes.size();

  bool restringido = false;
  //Si le pasamos un enlace como parametro solo carga las palabras claves
  if (argc==2){
    abiertos.push_back(argv[1]);
    if(Existe("palabrasclave.txt")){
      Cargar_PalabrasClave("palabrasclave.txt", palabras_clave_usadas);
    }
  }
  //Si no le pasamos nada como parametro sigue por donde iba, cargandolo todo.
  else if (Existe("abiertos.txt")){
    Cargar_Informacion("cerrados.txt", ya_existe);
    Cargar_Abiertos("abiertos.txt", abiertos);
    Cargar_Set("imagenes.txt", imagenes);
    Cargar_PalabrasClave("palabrasclave.txt", palabras_clave_usadas);
    if(Existe("palabra_enlaces.txt")){
      Cargar_MapMap("palabra_enlaces.txt", palabra_paginas);
    }
  }

  int t=0;
  //Mientras que queden nodos abiertos:
  while (!abiertos.empty() ){
    restringido = false;
    cout << "\tAbiertos: " << abiertos.size() << "\tCerrados: " << ya_existe.size() << "\tImagenes: " << imagenes.size() << endl;
    nodo = abiertos.front();//Cojemos el nodo
    //Comprobamos que no termina en /, ya que hay enlaces que son iguales pero con / al final y por lo tanto son repetidos.
    if((nodo[nodo.length()-1]) == '/'){
      nodo.erase(nodo.length()-1);
    }
    abiertos.erase(abiertos.begin());//borra ese nodo de abiertos
    Salvar_Abiertos("abiertos.txt", abiertos);

    auto it = ya_existe.find(nodo);//comprobamos que no este repetido
    if (it==ya_existe.end() and !restringido){//si no esta lo añadimos a la lista de cerrados
      t=0;
      par.first = nodo;
      par.second.titulo = nodo;
      par.second.enlace = nodo;
      par.second.repetido = 0;
      ya_existe.insert(par);//lo añadimos a los que existen
      Salvar_Informacion("cerrados.txt", ya_existe);
      llamada = comando+ "\""+nodo+"\" "+ final;//obtiene la pagina web y la guarda en fichero.html
      cout <<" Explorando:  "<< nodo << endl;
      system(llamada.c_str());//ejecuta la llamada
      cout << "Leido\n";
      pagina page("fichero.html");// MONTA TODOS LOS ARCHIVOS
      //cout << "Despues del constructor\n";
      //Explora el nodo
      page.Sacar_http(abiertos, imagenes, hay_contenido, palabras_clave_usadas, palabra_clave_encontrada);
      Salvar_PalabrasClave("palabrasclave.txt", palabras_clave_usadas);
      //cout<<"nodo antes de entrar al if"<<nodo<<endl;
      //cout<<"bool en el main: "<< hay_contenido << endl;
      //page.Lista_Literal_Notag();
      /*
      for (map<string,int>::iterator it=palabras_clave_usadas.begin(); it!=palabras_clave_usadas.end(); it++){
        cout << it->first <<"-MAIN-"<<it->second << endl;
      }
      */
      //Si encuentra contenido relevante en el nodo
      if(hay_contenido == true){
        pair<string, map<string, list<string>> > p_aux;
        pair<string, list<string>> p_auxdos,spm;

        dominio=ExtraerDominio(nodo);
        //Si el dominio no esta vacio
        url_pag_relevantes.insert(nodo);
        cout <<"ENCONTRADO CONTENIDO RELEVANTE"<< endl;
        cout<<"Añadido el nodo a la WEB: "<< nodo << endl;
        Salvar_Set("lista_pag_relevantes.txt", url_pag_relevantes);

        //cout<<"PALABRA CLAVE ENCONTRADA "<<palabra_clave_encontrada<<endl;
        //Buscamos la palabra clave que se ha encontrado y comprobamos si ya existe
        iter = palabra_paginas.find(palabra_clave_encontrada);

        //Si no existe añadimos un nuevo elemento al map con la palabra clave el dominio y el nodo
        if(iter==palabra_paginas.end()){
          //Añadir un elemento nuevo
          p_aux.first=palabra_clave_encontrada;
          spm.first=dominio;
          spm.second.push_back(nodo);
          p_aux.second.insert(spm);
          palabra_paginas.insert(p_aux);
          //cout<<"if"<<endl;
        }
        //Si existe:
        else{
          //buscamos si esta el dominio
          iterdos=iter->second.find(dominio);
          //Si no esta el dominio lo añadimos junto con el enlace
          if(iterdos==iter->second.end()){
            p_auxdos.first=dominio;
            p_auxdos.second.push_back(nodo);
            iter->second.insert(p_auxdos);
          }
          //si ya existe el dominio añadimos el enlace a ese dominio
          else{
            iterdos->second.push_back(nodo);
            /*cout<<"antes del else dentro del else"<<endl;
            for(map<string, list<string>>::const_iterator pp = iter->second.begin(); pp != iter->second.end(); pp++){
              for (list<string>::const_iterator p=pp->second.begin(); p!=pp->second.end(); p++){
                cout<< *p <<endl;
              }
            }
            cout<<"else dentro del else"<<endl;*/
          }
        }
        //Salvar_Map("palabra_enlaces.txt", palabra_paginas);
        Salvar_MapMap("palabra_enlaces.txt", palabra_paginas);
        /*
        for(map<string, list<string>>::const_iterator h = palabra_paginas.begin(); h != palabra_paginas.end(); ++h){
          cout<<"PALABRA"<<h->first<<endl;
          for (list<string>::const_iterator p=(h->second).begin(); p!=(h->second).end(); p++){
            cout<<"nodo: "<< *p <<endl;
          }
        }
        */
      }
      //No hay ningun contenido relevante
      else{
        cout <<"PAGINA NO RELEVANTE"<< endl;
      }
      cout<< "-------------------------------------------------------------------------------------------" << endl;
    }
    //El nodo ya ha sido explorado anteriormente
    else {
      if (!restringido)
        it->second.repetido++;
      t++;
      //cout << "Repetición numero " << t << endl;
      if (t>=20){
         Filtrar_Abiertos(abiertos, ya_existe);
         t=0;
       }
    }
    //Comprobamos si hay nuevas imagenes que añadir
    if (imagenes.size()>old_n_imagenes ){
      old_n_imagenes = imagenes.size();
      Salvar_Set("imagenes.txt", imagenes);
      body_imagenes = BodyI(imagenes);
      //Crear_html("imagenes.html", "", body_imagenes);
    }
    //Creamos la web
    //applet_html=Body(url_pag_relevantes);
    //Crear_html("applets.html","Lista de Páginas",applet_html);
    palabra_paginas_html=BodyMapMap(palabra_paginas);
    Crear_html("palabra_paginas.php","Resultados obtenidos",palabra_paginas_html,body_imagenes);

    //Pause();
  }

}
