#include <iostream>
#include <cstdio>
#include <string.h>

using namespace std;

const int DIM = 200;

struct orden
{
    unsigned int espera = 0;
    bool granada1 = false;
    bool granada2 = false;
    bool despegue = false;
    bool kamikaze = false;
    bool aterrizaje = false;
    int siguientex = -99;
    int siguientey = -99;
};

struct ordenarchivo
{
    unsigned int espera = 0;
    int x;
    int y;
    bool granada1 = false;
    bool granada2 = false;
    bool despegue = false;
    bool kamikaze = false;
    bool aterrizaje = false;
    int siguientex = 0;
    int siguientey = 0;
};

void inicializartabla(orden tabla[DIM][DIM])
{
    for(int i = 0; i < DIM; i++)
    {
        for(int k = 0; k < DIM; k++)
        {
            tabla[i][k].siguientey = -99;
            tabla[i][k].siguientex = -99;
            tabla[i][k].aterrizaje = false;
            tabla[i][k].granada1 = false;
            tabla[i][k].granada2 = false;
            tabla[i][k].kamikaze = false;
            tabla[i][k].despegue = false;
            tabla[i][k].espera = 0;
        }
    }
}

void cargartabla(char ruta[50], orden tabla[DIM][DIM])
{
    FILE* f;
    ordenarchivo x;
    int sgtx;
    int sgty;
    int cantidadaterrizaje = 0;
    int cantidaddespegue = 0;
    bool validosig = true;
    bool validoaterrizaje = true;
    bool despeguevalido = true;
    bool repite = false;
    bool paso = false;
    bool corresponden = true;
    int desx = -99;
    int desy = -99;

    bool aux[DIM][DIM];
    for(int r = 0; r < DIM; r++)
    {
        for(int e = 0; e < DIM; e++)
        {
            aux[r][e] = false;
        }
    }
    inicializartabla(tabla);
    f = fopen(ruta, "rb");
    if(f == NULL)
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    while(fread(&x, sizeof(ordenarchivo), 1, f))
    {
        if(x.despegue == true)
        {
            cantidaddespegue++;
            if(x.granada1 == true || x.granada2 == true || x.aterrizaje == true || x.kamikaze == true || x.espera>0)
            {
                despeguevalido = false;
            }
        }
        if(x.aterrizaje == true && x.kamikaze == true)
        {
            validoaterrizaje = false;
        }// si hay un despegue suma uno y si viene acompañado de una instruccion no es valido
        if(x.aterrizaje != x.kamikaze) // si los dos estan iguales cantidad de despejes no aumenta,ya que los dos juntos esta mal y si no hay ninguno cantidad nunca llega 1,ademas verifico que no vengan con ninguna instruccion
        {
            cantidadaterrizaje++;
            if(x.granada1 == true ||x.espera>0 || x.granada2 == true || x.despegue == true || x.siguientex != -99 || x.siguientey != -99)
            {
                validoaterrizaje = false;
            }
        }
    }

    if(cantidaddespegue > 1 || cantidaddespegue < 1)
    {
        despeguevalido = false;
    }
    if(cantidadaterrizaje > 1 || cantidadaterrizaje < 1)
    {
        validoaterrizaje = false;
    }

    fseek(f, 0, SEEK_SET);
    if(!despeguevalido || !validoaterrizaje)
    {
        cout << "El aterrizaje o el despegue estan mal-------" << endl;
    }

    if(despeguevalido && validoaterrizaje) //las validaciones solo las pongo en false si hubo un error ya que sino nunca las toco
    {
        while(fread(&x, sizeof(ordenarchivo), 1, f))
        {
            if(x.x >= 0 && x.x < DIM && x.y >= 0 && x.y < DIM)
            {
                if((tabla[x.x][x.y].siguientex == -99) && (tabla[x.x][x.y].siguientey == -99)) // aca corroboro que no se esten sobrescribiendo datos en la tabla
                {
                    tabla[x.x][x.y].aterrizaje = x.aterrizaje;
                    tabla[x.x][x.y].despegue = x.despegue;
                    tabla[x.x][x.y].granada1 = x.granada1;
                    tabla[x.x][x.y].granada2 = x.granada2;
                    tabla[x.x][x.y].kamikaze = x.kamikaze;
                    tabla[x.x][x.y].siguientex = x.siguientex;
                    tabla[x.x][x.y].siguientey = x.siguientey;
                    tabla[x.x][x.y].espera = x.espera;
                }
                else
                {
                    repite = true;
                }
            }
        }

        if(repite)
        {
            cout << "EL ARCHIVO TIENE POSICIONES REPETIDAS" << endl;
        }
        else
        {
            cout << x.x << "--" << x.y << endl;

        }

        if(!repite) //sino se repiten posiciones en los archivos la recorro para verificar que los x e y siguientes correspondan
        {
            for(int i = 0; i < DIM; i++)
            {
                for(int k = 0; k < DIM; k++)
                {
                    if(tabla[i][k].siguientex != -99 )
                    {
                        sgtx = tabla[i][k].siguientex;
                        sgty = tabla[i][k].siguientey;

                        if(sgtx >= 0 && sgtx < DIM && sgty >= 0 && sgty < DIM)
                        {
                            if(tabla[sgtx][sgty].aterrizaje == false && tabla[sgtx][sgty].kamikaze == false && tabla[sgtx][sgty].siguientex == -99) // aca pregunto si esta vacia la celda
                            {
                                corresponden = false;
                                cout << "no corresponde el x=" << i << " y=" << k << "con su siguiente x=" << sgtx << " y=" << sgty << endl;
                            }
                        }
                        else
                        {
                            corresponden=false;
                        }

                        if(corresponden) // si los x e y corresponden paso a validar que no haya huecos en el camino
                        {
                            if(tabla[i][k].aterrizaje == false && tabla[i][k].kamikaze == false)
                            {
                                if(((tabla[i][k].siguientex >= i - 1) && (tabla[i][k].siguientex <= i + 1)) &&
                                        ((tabla[i][k].siguientey >= k - 1) && (tabla[i][k].siguientey <= k + 1)))  //aca verifico en rango de donde puede estar el x e y siguiente para que no haya huecos
                                {
                                   /* cout << "de momento no hay huecos" << endl;*/
                                    cout << "el siguiente de x=" << i << "--y=" << k << "---es x=" << tabla[i][k].siguientex << "--y=" << tabla[i][k].siguientey << endl;
                                }
                                else
                                {
                                    validosig = false;
                                    cout << "hay espacios en la posicion=" << i << ";" << k << "-- y su siguiente es x=" << tabla[i][k].siguientex << "--y=" << tabla[i][k].siguientey << endl;
                                }
                            }
                        }
                    }
                }
            }

            if(validosig && corresponden && !repite) //si todas las demas validaciones estan bien
            {
                for(int t = 0; t < DIM; t++)  //determino la posicion de despegue
                {
                    for(int b = 0; b < DIM; b++)
                    {
                        if(tabla[t][b].despegue == true)
                        {
                            desx = t;
                            desy = b;
                            aux[t][b] = true;
                        }
                    }
                }

                while((!paso) && (desx != -99)) //recorro la tabla
                {
                    int sX = tabla[desx][desy].siguientex;
                    int sY = tabla[desx][desy].siguientey;

                    if((sX != -99 && sY != -99) && (aux[sX][sY] == false)) //verifico si paso ya por ahi
                    {
                        aux[sX][sY] = true; //marco como que paso
                        desx = sX;
                        desy = sY;
                    }
                    else
                    {
                        if((sX != -99) && (sY != -99)) //verifico que no sea un aterrizaje o ataque kamikaze
                        {
                            paso = true;
                        }
                        else
                        {
                            desx = -99;
                        }
                    }
                }

                if(paso)
                {
                    cout << "Paso dos veces por una posicion=" << endl;
                }
            }
        }
        if(paso || repite || !validosig || !despeguevalido || !validoaterrizaje)
        {
            inicializartabla(tabla);
        }

    }
    fclose(f);
}

// Visualizar tabla en HTML
void visualizarArchivoHtml(char *nombreArchivo, orden tabla[DIM][DIM])
{
    FILE *archivo = fopen((strcat(nombreArchivo, ".html")), "w");
    if (archivo == NULL)
    {
        cout << "No se pudo crear el archivo HTML." << endl;
        return;
    }

    fprintf(archivo, "<!DOCTYPE html>\n");
    fprintf(archivo, "<html lang=\"es\">\n");
    fprintf(archivo, "<head>\n");
    fprintf(archivo, "  <meta charset=\"UTF-8\">\n");
    fprintf(archivo, "  <title>Sistema de Guia de Apoyo - Dron Militar</title>\n");

    // Estilos del html, antes del body para ahorrar posibles fallas
    fprintf(archivo, "  <style>\n");
    fprintf(archivo, "    body {\n");
    fprintf(archivo, "        background-color: #0d1117;\n");
    fprintf(archivo, "        color: #c9d1d9;\n");
    fprintf(archivo, "        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n");
    fprintf(archivo, "        margin: 0;\n");
    fprintf(archivo, "        padding: 20px;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .container {\n");
    fprintf(archivo, "        max-width: 1400px;\n");
    fprintf(archivo, "        margin: 0 auto;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    header {\n");
    fprintf(archivo, "        text-align: center;\n");
    fprintf(archivo, "        margin-bottom: 20px;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    header h1 {\n");
    fprintf(archivo, "        color: #58a6ff;\n");
    fprintf(archivo, "        margin-bottom: 5px;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .main-content {\n");
    fprintf(archivo, "        display: flex;\n");
    fprintf(archivo, "        gap: 20px;\n");
    fprintf(archivo, "        justify-content: center;\n");
    fprintf(archivo, "        align-items: flex-start;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .sidebar {\n");
    fprintf(archivo, "        background-color: #161b22;\n");
    fprintf(archivo, "        border: 1px solid #30363d;\n");
    fprintf(archivo, "        border-radius: 6px;\n");
    fprintf(archivo, "        padding: 15px;\n");
    fprintf(archivo, "        width: 200px;\n");
    fprintf(archivo, "        flex-shrink: 0;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .sidebar h3 {\n");
    fprintf(archivo, "        margin-top: 0;\n");
    fprintf(archivo, "        font-size: 15px;\n");
    fprintf(archivo, "        border-bottom: 1px solid #30363d;\n");
    fprintf(archivo, "        padding-bottom: 8px;\n");
    fprintf(archivo, "        color: #f0f6fc;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .ref-item {\n");
    fprintf(archivo, "        display: flex;\n");
    fprintf(archivo, "        align-items: center;\n");
    fprintf(archivo, "        gap: 10px;\n");
    fprintf(archivo, "        margin-bottom: 10px;\n");
    fprintf(archivo, "        font-size: 13px;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .dot {\n");
    fprintf(archivo, "        width: 12px;\n");
    fprintf(archivo, "        height: 12px;\n");
    fprintf(archivo, "        border-radius: 3px;\n");
    fprintf(archivo, "        display: inline-block;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .grid-container {\n");
    fprintf(archivo, "        background-color: #161b22;\n");
    fprintf(archivo, "        border: 1px solid #30363d;\n");
    fprintf(archivo, "        border-radius: 6px;\n");
    fprintf(archivo, "        overflow: auto;\n");
    fprintf(archivo, "        width: 750px;\n");
    fprintf(archivo, "        height: 650px;\n");
    fprintf(archivo, "        padding: 15px;\n");
    fprintf(archivo, "        box-sizing: border-box;\n");
    fprintf(archivo, "        display: block;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .grid-200 {\n");
    fprintf(archivo, "        display: grid;\n");
    fprintf(archivo, "        grid-template-columns: repeat(200, 14px);\n");
    fprintf(archivo, "        grid-template-rows: repeat(200, 14px);\n");
    fprintf(archivo, "        gap: 1px;\n");
    fprintf(archivo, "        background-color: #30363d;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .cell {\n");
    fprintf(archivo, "        width: 14px;\n");
    fprintf(archivo, "        height: 14px;\n");
    fprintf(archivo, "        border: 0.5px solid #21262d;\n");
    fprintf(archivo, "        box-sizing: border-box;\n");
    fprintf(archivo, "        cursor: pointer;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .cell:hover {\n");
    fprintf(archivo, "        border-color: #58a6ff;\n");
    fprintf(archivo, "        z-index: 5;\n");
    fprintf(archivo, "    }\n");
    fprintf(archivo, "    .cell.vacio { background-color: #0d1117; }\n");
    fprintf(archivo, "    .cell.ruta, .dot.ruta { background-color: #30363d; }\n");
    fprintf(archivo, "    .cell.despegue, .dot.despegue { background-color: #238636; }\n");
    fprintf(archivo, "    .cell.kamikaze, .dot.kamikaze { background-color: #da3633; }\n");
    fprintf(archivo, "    .cell.aterrizaje, .dot.aterrizaje { background-color: #ba68c8; }\n");
    fprintf(archivo, "    .cell.granada, .dot.granada { background-color: #9e6a03; }\n");
    fprintf(archivo, "    .cell.espera, .dot.espera { background-color: #1f6feb; }\n");
    fprintf(archivo, "  </style>\n");

    fprintf(archivo, "</head>\n");
    fprintf(archivo, "<body>\n");
    fprintf(archivo, "  <div class=\"container\">\n");
    fprintf(archivo, "    <header><h1>Sistema de Guia de Apoyo - Dron Militar</h1></header>\n");
    fprintf(archivo, "    <main class=\"main-content\">\n");

    // Panel lateral de Referencias
    fprintf(archivo, "      <div class=\"sidebar\">\n");
    fprintf(archivo, "        <h3>Referencias</h3>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot ruta\"></span> Movimiento / Ruta</div>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot despegue\"></span> Despegue (Origen)</div>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot granada\"></span> Soltar Granada</div>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot espera\"></span> Espera / Pausa</div>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot kamikaze\"></span> Ataque Kamikaze (Fin)</div>\n");
    fprintf(archivo, "        <div class=\"ref-item\"><span class=\"dot aterrizaje\"></span> Aterrizaje (Fin)</div>\n");
    fprintf(archivo, "      </div>\n");

    // Contenedor de la tabla
    fprintf(archivo, "      <div class=\"grid-container\">\n");
    fprintf(archivo, "        <div class=\"grid-200\">\n");

    for (int fila = 0; fila < DIM; fila++)
    {
        for (int columna = 0; columna < DIM; columna++)
        {
            const orden &o = tabla[fila][columna];
            string claseCss = "vacio";

            if (o.despegue) claseCss = "despegue";
            else if (o.kamikaze) claseCss = "kamikaze";
            else if (o.aterrizaje) claseCss = "aterrizaje";
            else if (o.granada1 || o.granada2) claseCss = "granada";
            else if (o.espera > 0) claseCss = "espera";
            else if (o.siguientex != -99 || o.siguientey != -99) claseCss = "ruta";

            fprintf(archivo, "          <div class=\"cell %s\"></div>\n", claseCss.c_str());
        }
    }

    fprintf(archivo, "        </div>\n");
    fprintf(archivo, "      </div>\n");
    fprintf(archivo, "    </div>\n");
    fprintf(archivo, "  </div>\n"); //container
    fprintf(archivo, "</body>\n");
    fprintf(archivo, "</html>\n");

    fclose(archivo);
    cout << "¡Archivo HTML '" << nombreArchivo << "' generado con exito!" << endl;
}
void corregirtabla(orden tabla[DIM][DIM])
{
    int x;
    int y;
    orden k;
    cout << "Ingrese la posicion de la tabla que desea modificar" << endl;
    cout << "x=";
    cin >> x;
    cout << "y=";
    cin >> y;
    if(x >= 0 && x < DIM && y >= 0 && y < DIM)
    {
        cout << "----- INGRESE LOS NUEVOS ELEMENTOS ---" << endl;
        cout << "ingrese el siguiente x=";
        cin >> k.siguientex;
        cout << "ingrese el siguiente y=";
        cin >> k.siguientey;
        cout << "ingrese si hay despegue (1/0): ";
        cin >> k.despegue;
        cout << "ingrese si quiere lanzar granada 1 (1/0): ";
        cin >> k.granada1;
        cout << "ingrese si quiere lanzar granada 2 (1/0): ";
        cin >> k.granada2;
        cout << "ingrese la espera=";
        cin >> k.espera;
        cout << "ingrese si quiere realizar un ataque kamikaze (1/0): ";
        cin >> k.kamikaze;
        cout << "ingrese si quiere aterrizar (1/0): ";
        cin >> k.aterrizaje;
        tabla[x][y] = k;
    }
    else
    {
        cout << "Esos indices no existen" << endl;
    }
}
void corregirarchivo(char ruta[50])
{
    FILE*f;
    int x;
    int y;
    ordenarchivo k;
    bool encontro=false;
    if(f=fopen(ruta,"rb+"))
    {
        cout<<"Ingrese las coordenadas del regitro del archivo que desea modificar"<<endl;
        cout<<"x=";
        cin>>x;
        cout<<"y=";
        cin>>y;
        while((fread(&k,sizeof(ordenarchivo),1,f))&& !encontro)
        {
            if((k.x==x) && (k.y==y))
            {
                cout<<k.x<<endl;
                cout<<"ingrese los datos del nuevo registro"<<endl;
                cout<<"ingrese la coordenada x=";
                cin>>k.x;
                cout<<"ingrese la coordenada en y=";
                cin>>k.y;
                cout<<"ingrese el siguiente x=";
                cin>>k.siguientex;
                cout<<"ingrese el siguiente y=";
                cin>>k.siguientey;
                cout<<"ingrese si hay despegue:";
                cin>>k.despegue;
                cout<<"ingrese si quiere lanzar granada:";
                cin>>k.granada1;
                cout<<"ingrese si quiere lanzar granada2:";
                cin>>k.granada2;
                cout<<"ingrese si desea esperar:";
                cin>>k.espera;
                cout<<"ingrese si quiere realizar un ataque kamikaze:";
                cin>>k.kamikaze;
                cout<<"ingrese si quiere aterrizar:";
                cin>>k.aterrizaje;
                fseek(f,(-1)*sizeof(ordenarchivo),SEEK_CUR);
                fwrite(&k,sizeof(ordenarchivo),1,f);
                fseek(f,0,SEEK_CUR);
                encontro=true;
            }
        }
        fclose(f);
    }
    else
    {
        cout<<"No se pudo abrir el archivo"<<endl;

    }
    fclose(f);
}
void guardartablaenarchivo(orden tabla[DIM][DIM], char ruta[50])
{
    FILE* f;
    ordenarchivo x;
    if(f = fopen(ruta, "wb"))
    {
        for(int i=0; i<DIM; i++)
        {
            for(int k=0; k<DIM; k++)
            {
                if(tabla[i][k].siguientex!=-99 && tabla[i][k].siguientey!=-99 || tabla[i][k].aterrizaje==true || tabla[i][k].kamikaze==true || tabla[i][k].despegue==true)
                {
                    x.aterrizaje=tabla[i][k].aterrizaje;
                    x.despegue=tabla[i][k].despegue;
                    x.espera=tabla[i][k].espera;
                    x.granada1=tabla[i][k].granada1;
                    x.granada2=tabla[i][k].granada2;
                    x.kamikaze=tabla[i][k].kamikaze;
                    x.siguientex=tabla[i][k].siguientex;
                    x.siguientey=tabla[i][k].siguientey;
                    x.x=i;
                    x.y=k;
                    fwrite(&x,sizeof(ordenarchivo),1,f);
                }
            }
        }

    }
    else
    {
        cout<<"no se pudo abrir el archivo"<<endl;
    }
    fclose(f);
}

void mostrartabla(orden tabla[DIM][DIM])
{
    int cantlineas = 0;
    cin.ignore(10000, '\n');
    for(int i = 0; i < DIM; i++)
    {
        for(int k = 0; k < DIM; k++)
        {
            if((tabla[i][k].siguientex != -99 && tabla[i][k].siguientey != -99) || (tabla[i][k].kamikaze || tabla[i][k].aterrizaje || tabla[i][k].despegue))
            {
                cantlineas++;
                cout << "x=" << i << " y=" << k << endl;
                if(tabla[i][k].despegue)
                    cout << "DESPEGO" << endl;
                if(tabla[i][k].espera)
                    cout << "ESPERA" << endl;
                if(tabla[i][k].granada1)
                    cout << "TIRO GRANADA 1" << endl;
                if(tabla[i][k].granada2)
                    cout << "TIRO GRANADA 2" << endl;
                if(tabla[i][k].aterrizaje)
                    cout << "ATERRIZO" << endl;
                if(tabla[i][k].kamikaze)
                    cout << "ATAQUE KAMIKAZE" << endl;
                if(cantlineas >= 20)
                {
                    cout << "desea pasar a otra pagina(pulse enter)?";
                    cin.get();
                    system("cls");
                    cantlineas = 0;
                }
            }
        }
    }
}
void  creararchivo(char ruta[50])
{
    FILE*f;
    ordenarchivo k;
    bool finalizo=false;
    if(f=fopen(ruta,"wb"))
    {
        while(!finalizo)
        {
            cout<<"Ingrese la coordenada x=";
            cin>>k.x;
            cout<<"Ingrese la coordenada en y=";
            cin>>k.y;
            cout<<"Ingrese el siguiente x=";
            cin>>k.siguientex;
            cout<<"Ingrese el siguiente y=";
            cin>>k.siguientey;
            cout<<"Ingrese si hay despegue:";
            cin>>k.despegue;
            cout<<"Ingrese si quiere lanzar granada;";
            cin>>k.granada1;
            cout<<"Ingrese si quiere lanzar granada 2";
            cin>>k.granada2;
            cout<<"Ingrese si desea esperar:";
            cin>>k.espera;
            cout<<"Ingrese si quiere realizar un ataque kamikaze:";
            cin>>k.kamikaze;
            cout<<"Ingrese si quiere aterrizar:";
            cin>>k.aterrizaje;
            fwrite(&k,sizeof(ordenarchivo),1,f);
            cout<<"Quiere finalizar de ingresar registros?(1-si 0-no)";
            cin>>finalizo;
        }
        fclose(f);
    }
}
void mostrararchivo(char ruta[50])
{
    FILE*f;
    ordenarchivo x;
    if(f=fopen(ruta,"rb"))
    {
        while(fread(&x,sizeof(ordenarchivo),1,f))
        {
            cout<<"x="<<x.x<<endl;
            cout<<"y="<<x.y<<endl;
            if(x.despegue)
                cout<<"DESPEGO"<<endl;
            if(x.aterrizaje)
                cout<<"ATERRIZO"<<endl;
            if(x.granada1)
                cout<<"LANZO GRANADA 1"<<endl;
            if(x.granada2)
                cout<<"LANZO GRANADA 2"<<endl;
            if(x.kamikaze)
                cout<<"REALIZO UN ATAQUE KAMIKAZE"<<endl;
            cout<<"EL TIEMPO DE ESPERA ES="<<x.espera<<endl;

        }
    }
}

int mostrarmenu(int x)
{
    cout<<"1-CARGAR ARCHIVO DE ATAQUE EN MEMORIA"<<endl;
    cout<<"2-MOSTRAR ATAQUE"<<endl;
    cout<<"3-CREAR UN ARCHIVO DE ATAQUE NUEVO"<<endl;
    cout<<"4-CORREGIR UN REGISTRO DEL ARCHIVO"<<endl;
    cout<<"5-CORREGIR UN REGISTRO DE MEMORIA"<<endl;
    cout<<"6-GUARDAR MEMORIA EN UN ARCHIVO"<<endl;
    cout<<"7-VISUALIZAR TABLA HTML"<<endl;
    cout<<"8-MOSTRAR ARCHIVO"<<endl;
    cin>>x;
    return(x);
}

int main()
{
    FILE*f;
    orden tabla[DIM][DIM];
    ordenarchivo x;
    char ruta[50];
    int y;
    int p;
    bool parar=false;

    do
    {
        p=mostrarmenu(y);
        switch(p)
        {
        case 1:
        {
            cout<<"Ingrese la ruta del archivo a leer:";
            cin>>ruta;
            cargartabla(ruta,tabla);
        }
        break;
        case 2:
        {
            mostrartabla(tabla);
        }
        break;
        case 3:
        {
            cout<<"Ingrese la ruta del archivo:";
            cin>>ruta;
            creararchivo(ruta);
        }
        break;
        case 4:
        {
            cout<<"Ingrese la ruta del archivo a corregir:";
            cin>>ruta;
            corregirarchivo(ruta);
        }
        break;
        case 5:
        {
            corregirtabla(tabla);
        }
        break;
        case 6:
        {
            guardartablaenarchivo(tabla,ruta);
        }
        break;
        case 7:
        {
            cout<<"Ingrese la ruta del archivo:";
            cin>>ruta;
            visualizarArchivoHtml(ruta,tabla);
        }
        break;
        case 8:
        {
            cout<<"Ingrese la ruta del archivo:";
            cin>>ruta;
            mostrararchivo(ruta);
        }
        break;

        case 0:
        {
            parar=true;
        }
        default:
        {
            cout<<"NO HAY NINGUNA OPERACION PARA ESE INDICE"<<endl;
        }
        break;
        }
    }
    while(!parar);


    return 0;
}
