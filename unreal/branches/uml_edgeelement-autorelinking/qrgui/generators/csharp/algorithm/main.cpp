#include <QtCore/QCoreApplication>
#include <QtCore>
#include <QtGlobal>
#include "graph.h"
#define IF 1
#define ACTION 0
#define WHILE 3
#define COMPOSITE 4
#define MERGE 2


struct merge{
    //QList<int> list;//список входящих ребер
    int type;//type = 1 - обычная строка, type=2 - Merge
    int parent_type; // if или while или ничего
    int id;
    int next; //здесь номер в списке у того, кто оборвал
    int killer;//убица
    bool life; //жив или мертв
    QList<merge> nodes;
    QString name;
    int idlast;
    int whilekiller;
};

/*содержится ли id в mr*/
bool contains(merge mr, int id){
    bool key = false;
    foreach(merge mr2, mr.nodes){
        if (mr2.id == id)
            key = true;
    }
    return key;
}

/*правда ли мы можем убить?*/
bool realKill(QList<merge> M, int i, int killer){
    int a = killer;
    while(!M[a].life){
;
    }

}

/*исправляем живучесть*/
QList<merge> isLife(QList<merge> M){
    qDebug()<<"KILLERS!";
    for(int i=0; i < M.length(); i++){
        qDebug()<<i;
        if(M[i].life){
            for(int j = 0; j < M.length(); j++){
                qDebug()<<j;
                if( contains(M[j],M[i].nodes.last().id) && (i!=j) && (M[j].killer !=  M[i].id) ) {
                    qDebug()<<"KILLL";
                    qDebug()<<i<<j<<M[i].nodes.last().id;

                    qDebug()<<";";
                    M[i].life = false;
                    M[i].killer = M[j].id;
                    for(int z = 0; z < M.length(); z++){
                        if(M[j].nodes[z].id == M[i].nodes.last().id){
                            M[i].next = z;
                            z = M.length()+6;
                        }
                    }
                }
            }
            qDebug()<<"";
        }
    }
   return M
           ;
}

int numberLife(QList<merge> M){
    int k = 0;
    if(!M.isEmpty()){
        for(int i=0; i<M.length();i++)
            if(M.at(i).life)
                k++;
    }
    return k;
}

/*ищем ближайщий merdge по id ребра и вершины*/
int searchMerge(graph gr, int edge, int node){
    int k =  gr.getAnotherEnd(edge, node);
    while( gr.getInlinks(k).length()<2){
       int a= gr.getOutlinks(k).first();
       k = gr.getAnotherEnd(a, k);
    }
    return k;
}
merge searchparent(QList<merge> mr, int parentid){
    int key=0;
    merge m;
    foreach(merge mr2, mr){
        if(mr2.id ==parentid)
            return mr2;
    }
    foreach(merge mr2, mr){
        merge mr;
        mr = searchparent(mr2.nodes,parentid);
        if(mr.id==parentid){
            m = mr;
            key = 1;
            break;
        }
    }
    if(key==0){
      m.id = 0;
    }
    return m;
}

QString print(QList<merge> mr,QList<merge> all, int num, bool whil ){
    QString tab ="";
    QString whilestrcondition="";
    bool whileexist = false;
    bool ifexist = false;
    QString whilestr ="";
    QString ifstr ="";
    QString act="";
    QString result ="";
    int ch=0;
    int key=0;

    for(int i=0; i<num; i++){
        tab+="  ";
    }
    foreach(merge mr2, mr){
      
         QString str;
        str.setNum(mr2.type);
       // result+=str;
        str.setNum(whil);
      //  result+=str;
       // qDebug()<<whil;
        if(mr2.type==COMPOSITE){
             whil=true;
         key=1;
         }
        if(mr2.type==1){
            act+=tab+mr2.name+";\n";
            ch=1;
        }else if(mr2.type==2 || mr2.type == WHILE || mr2.type == COMPOSITE)
            ch =2;
             if(mr2.parent_type==IF && mr2.type != COMPOSITE){
                if(ifexist) ifstr += tab+"else ";
                ifstr += "if("+mr2.name+")\n"+tab+"{\n"+print(mr2.nodes, all, num+2,whil);
                if(!mr2.life){
                    merge killer;
                    killer = searchparent(all, mr2.killer);
                    QList<merge> endprint;
                    bool ch=false;
                    for(int i=0; i< killer.nodes.size(); i++){
                        if(ch)
                            endprint<<killer.nodes[i];
                        if(killer.nodes[i].id==mr2.nodes.last().id){
                            ch = true;
                        }

                    }
                    ifstr += print(endprint, all, num+2,whil);
                    ifexist = true;

                }
                if(whil && mr2.type  )
                    ifstr+=tab+"break;\n";

                ifstr +=tab+"}\n";
            }else if(mr2.parent_type==WHILE || mr2.type == COMPOSITE){
                if(whileexist){
                    whilestrcondition +=" || ";
                    whilestr +=tab+"else ";
                }
                whilestrcondition = "("+mr2.name+")";

                    whilestr +=tab+"if("+mr2.name+")\n"+tab+"{\n"+print(mr2.nodes, all,num+2,whil)+tab+"}\n";
                    whileexist = true;

            }else{
              ch=1;
              act += print(mr2.nodes, all, num,whil);
            }

    }
   /* if(whil){
        result+="YAHOO";
   }
   */
    if(ch==2){
        if(whil & key==0){
            result+=tab+"{"+whilestr+tab+"}\n";
        }else if(whileexist)
            result+=tab+"while("+whilestrcondition+")\n"+tab+"{"+whilestr+tab+"}\n";

         result+=tab+ifstr;
    }else
        result+=act;

    return result;

}
void writ(QList<merge> mr){
    qDebug()<<"WRITE";
    foreach(merge mr2, mr){
        if(mr2.type==1){
            qDebug()<<mr2.name;
        }else{
            qDebug()<<"type2";
            writ(mr2.nodes);

        }
     }
    qDebug()<<";";
}


/*хочу найти рекурсивно кто убил*/
/*int searchlastkiller(int stop, QList<merge> M){
for(int j=0; j < M.length();j++ ){
    if(M[j].killer == M[stop].id){
        if(M[j].next > max)
            max = M[j].next;
    }
}
}*/
QList<merge> findMerge(graph gr, QList<QString> list, QList<int> stopList){
    QList<merge> M;

qDebug()<<"FINDMERGE BEGIN";
    /*делаем первое приближение к искомому мерджу*/
    foreach (int i, gr.getOutlinks(stopList.last())){
       int k =  gr.getAnotherEnd( i, stopList.last());
       merge mr;
       merge mr1;
       mr1.type = 1;
       mr1.name = list[k];
       mr1.id = k;
       mr.nodes << mr1;
       mr.id = i;
       mr.next = -1;
       mr.life = true;
       mr.type = 2;

       mr.parent_type = IF;

       QString str;
       str.setNum(stopList.last());
       str+="-";
       mr.name = str;
       str.setNum(k);
       mr.name += str;
       M<<mr;
    }
    M = isLife(M);




    /*поиск решения*/
while(numberLife(M)>1){
        /*новое приблежение*/
    qDebug()<<"NEW STEP";

    for(int i = 0; i < M.length(); i++){
            qDebug()<<i;
            if(M[i].life){
                qDebug()<<M[i].nodes.last().name;
              if(!gr.getOutlinks(M[i].nodes.last().id).isEmpty()){
                    int edge = gr.getOutlinks(M[i].nodes.last().id).first();
                    int next =  gr.getAnotherEnd(edge, M[i].nodes.last().id);
                    merge mr2;
                    mr2.parent_type = ACTION;
                    qDebug()<<"NUMBER LINKS";
                    qDebug()<<gr.getOutlinks(next).size();
                    qDebug()<<next;
                    qDebug()<<stopList;
                    qDebug()<<"";

                    if(stopList.contains(next)){
                        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!stoplist!!!!!!!!!!!!!!!!!!!!!!!!!! ";
                        M[i].whilekiller=0;
                        qDebug()<<"WHILE!!!!!!!";
                        M[i].type = WHILE;
                        M[i].parent_type = WHILE;
                        M[i].life = false;
                        M[i].killer = -1;
                        qDebug()<<stopList;
                        qDebug()<<next;
                        if(next != stopList.last()){
                            qDebug()<<"***********************search killer!!!";
                            for(int i=0; i < stopList.length(); i++){
                                if(next == stopList[stopList.length()-i-1]){
                                    M[i].whilekiller = i;
                                    qDebug()<<"!!!!!!!!!!!!!!!!!!killer";
                                    qDebug()<<i;
                                }
                            }
                       }
                    }else if(gr.getOutlinks(next).size()>1){
                        qDebug()<<"MERGE!!\n";
                        qDebug()<<"YYYYYYEEA!!!!!";
                        stopList<<next;
                        mr2.nodes = findMerge(gr, list, stopList);
                        mr2.type = 2;

                        if(mr2.nodes.last().type == WHILE){
                            mr2.parent_type == WHILE;
                            qDebug()<<"While";
                        }

                        int max=-1;
                        foreach(merge m, mr2.nodes){
                            if((m.type == WHILE || m.type == COMPOSITE )&& m.whilekiller>max )
                            {
                                qDebug()<<"max!!!!!!!!!!!!!";
                                qDebug()<< m.whilekiller;
                                max = m.whilekiller;
                            }
                        }
                        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!max";
                        qDebug()<<max;
                        if (max>-1){
                            M[i].type = COMPOSITE;
                            M[i].whilekiller = max-1;
                        }   

                        for(int i=0; i<mr2.nodes.length();i++){
                          if(mr2.nodes[i].life){
                            mr2.id = mr2.nodes[i].nodes.last().id;
                            i = mr2.nodes.length();
                          }
                        }

                    }else{
                       mr2.name = list[next];
                       mr2.id = next;
                       mr2.type=1;
                    }
                       M[i].nodes<< mr2;
                }
        }
        }

        /*список живых*/
       M = isLife(M);
    }


 // writ(M);
    /*search stop node*/
  /*   int max = -1;
      int stopnode; // id того, кто жив

      for(int i=0; i<M.length();i++){
        if(M[i].life){
            stopnode = i;
            for(int j=0; j < M.length();j++ ){
                if(M[j].killer == M[i].id){
                    if(M[j].next > max)
                        max = M[j].next;
                }

            }
            break;
        }
    }
    *///  qDebug()<<stopnode;
     // qDebug()<<max;
     /* for(int i = 0; i<M[stopnode].nodes.length(); i++){
        qDebug()<<M[stopnode].nodes[i].name;
      }
     /* for(int j=M[stopnode].nodes.length(); j > max+1; j--){
        M[stopnode].nodes.removeLast();
      }*/

 // writ(M);

  qDebug()<<"FINDMERGE END";
  qDebug()<<"\n\n";
  qDebug()<< print(M, M, 0,false);
  return M;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QList<QString> list;
    list<<"0"<<"01"<<"02"<<"03"<<"04"<<"05"<<"06"<<"07"<<"08"<<"09"<<"10"<<"11"<<"12"<<"13"<<"no"<<"no"<<"no";
    graph Graph;
    Graph.link(0, 1);
    Graph.link(0, 2);
    Graph.link(1, 9);
    Graph.link(2, 3);
    Graph.link(3, 4);
    Graph.link(3, 7);
    Graph.link(7, 8);
    Graph.link(8, 9);
    Graph.link(4, 5);
    Graph.link(5, 10);
    Graph.link(10, 11);
    Graph.link(11, 2);
    Graph.link(5, 6);
    Graph.link(6, 8);
   // Graph.link(4, 2);
  //  Graph.link(0, 6);
 //   Graph.link(6, 12);
  //  Graph.link(12, 0);
   // Graph.link(7, 8);
   // Graph.link(8, 9);
   // Graph.link(9, 10);
   // Graph.link(10, 11);



    qDebug()<<Graph.toQString();
    QList<int> stoplist;
    stoplist<<0;
    QList<merge> m = findMerge(Graph,list,stoplist);
    return a.exec();
}
