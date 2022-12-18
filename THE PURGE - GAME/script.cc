#include "Player.hh"



/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME aNaKin31
 


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  typedef vector<int> Row;
  typedef vector<Row> MatrixD;

  

  /**
   * Play method, invoked once per each round.
   */
  #define inf 2147483647 //definim el valor enter màxim en c++ como 'inf' (infinit)


  const vector <Dir> dirs = {Down, Up, Right, Left};
  MatrixD dist_money;
  MatrixD dist_enemies;
  MatrixD dist_food;
  MatrixD dist_weapon; 
  MatrixD dist_moneyw;
  vector <Pos> def;
  vector<int> bld;
  vector<int> war;

 /* 
 * @brief   Funció booleana que ens indica si existeix un enemic proper a la nostra posició p.
 * @param   Pos p
 * @return  Donada una posicio p valida, ens retorna cert si hi ha un enemic en les 4 posicions adjacents a p.
 *          En cas contrari, retorna fals.
 */
  bool near_enem(Pos p) {
      for(Dir x : dirs) {
        if(pos_ok(p+x) and citizen(cell(p+x).id).player != me()) return true;
      }
      return false;
  }


  /*
  * @brief Funció que ens comprova si tenim defensa en forma de barricades.
  * @param 
  * @return Ens retorna cert si tenim alguna barricada construida, en cas contrari retorna fals.
  */ 
  bool have_defense(){
      if(def.size() > 0) return true;
      return false;
  }  

  /*
  * @brief Funció que ens comprova si tots els nostres guerreres tenen les millors armes possibles.
  * @param 
  * @return Ens retorna cert si tots els guerrers tenen el Bazooka, en cas contrari retorna fals.
  */ 
  bool best_weapons() {
      for(int id : war) {
          if(citizen(id).weapon != Bazooka) return false;
      }
      return true;
  }

Dir moneyw_search(Pos p) {
    dist_moneyw[p.i][p.j] = 0; //la distancia on està el nostre jugador la posem a 0
    queue<Pos> cua;
    cua.push(p);
  
    

    //mentre no estigui buida la cua
    while(not cua.empty()) {
        Pos aux = cua.front();
        cua.pop();

        if(pos_ok(aux) and cell(aux).id == -1 and cell(aux).bonus == Money) { //si hem trobat el bonus Money
            while(dist_moneyw[aux.i][aux.j] != 0) { //mentre la distància sigui diferent a 0, no hem trobat la posició inicial on estem
                    /* 
                    Si en una posició adjacent trobem que està a menys distancia de la posició aux, actualitzem aux.
                    Arribarem al final quan trobem la posició inicial en la que està el jugador, la posició referent 
                    amb distància igual a 0.
                    */
                    for(Dir d : dirs) {
                        Pos aux_2 = aux + d;
                        if(pos_ok(aux_2) and cell(aux_2).type == Street and  (dist_moneyw[aux_2.i][aux_2.j]+1) == dist_moneyw[aux.i][aux.j]) aux = aux_2;
                      
                        if(dist_moneyw[aux.i][aux.j] == 0) {
                            if(d == Down)  return dirs[1];
                            if(d == Up)    return dirs[0];
                            if(d == Left)  return dirs[2];
                            if(d == Right) return dirs[3];
                        }
                    }
            }
        }
        else {
            /*
            Mirem les posicions adjacents a aux, si alguna te valor inf, actualitzem la distancia amb
            distancia(aux)+1 i afegim la posicio a la cua.
            */
            for(Dir d : dirs) {
                if(pos_ok(aux+d) and cell(aux+d).id == -1 and cell(aux+d).type == Street) {
                    Pos next = aux+d;
                    if(dist_moneyw[next.i][next.j] == inf) {
                      cua.push(next);
                      dist_moneyw[next.i][next.j] = dist_moneyw[aux.i][aux.j] + 1;
                    }
                }
            }
        }
    }
     //Si la vida que té el nostre jugador és menor que la inicial, busquem menjar
     if(citizen(cell(p).id).type == Warrior and citizen(cell(p).id).life < warrior_ini_life()) {
              return food_search(p);
     } 
     //Retornem la posició on no hi hagi cap jugador i sigui carrer, si a la seguent posició hi ha edifici, retornem posició random per no produir un bloqueig al jugador.
     else {
        for(Dir d : dirs) {
            if(pos_ok(p+d) and cell(p+d).type == Street) {
                Pos x = p+d;
                if(pos_ok(x+d) and cell(x+d).type == Building) {
                    return dirs[random(0,3)];
                }
                else {
                    return d;
                }
            }
        }
     }

     //Com a ultima opció, retornem una direcció random entre {Down, Up, Right, Left}
     return dirs[random(0,3)];
  }

  Dir money_search(Pos p) {
    dist_money[p.i][p.j] = 0; //la distancia on està el nostre jugador la posem a 0
    queue<Pos> cua;
    cua.push(p);
  
    

    //mentre no estigui buida la cua
    while(not cua.empty()) {
        Pos aux = cua.front();
        cua.pop();

        if(pos_ok(aux) and cell(aux).id == -1 and cell(aux).bonus == Money) { //si hem trobat el bonus Money
            while(dist_money[aux.i][aux.j] != 0) { //mentre la distància sigui diferent a 0, no hem trobat la posició inicial on estem
                    /* 
                    Si en una posició adjacent trobem que està a menys distancia de la posició aux, actualitzem aux.
                    Arribarem al final quan trobem la posició inicial en la que està el jugador, la posició referent 
                    amb distància igual a 0.
                    */
                    for(Dir d : dirs) {
                        Pos aux_2 = aux + d;
                        if(pos_ok(aux_2) and cell(aux_2).type == Street and  cell(aux_2).weapon == NoWeapon and (dist_money[aux_2.i][aux_2.j]+1) == dist_money[aux.i][aux.j]) aux = aux_2;
                      
                        if(dist_money[aux.i][aux.j] == 0) {
                            if(d == Down)  return dirs[1];
                            if(d == Up)    return dirs[0];
                            if(d == Left)  return dirs[2];
                            if(d == Right) return dirs[3];
                        }
                    }
            }
        }
        else {
            /*
            Mirem les posicions adjacents a aux, si alguna te valor inf, actualitzem la distancia amb
            distancia(aux)+1 i afegim la posicio a la cua.
            */
            for(Dir d : dirs) {
                if(pos_ok(aux+d) and cell(aux+d).id == -1 and  cell(aux+d).weapon == NoWeapon and cell(aux+d).type == Street) {
                    Pos next = aux+d;
                    if(dist_money[next.i][next.j] == inf) {
                      cua.push(next);
                      dist_money[next.i][next.j] = dist_money[aux.i][aux.j] + 1;
                    }
                }
            }
        }
    }
     //Si es de dia i es la primera ronda, construim barricades si es pot
    if(is_day() and not have_defense() and round()%num_rounds_per_day() == 2) {
         for(Dir d : dirs) {
            if(pos_ok(p+d) and cell(p+d).is_empty()) build(cell(p).id, d);
         }
     }
     //Si la vida que té el nostre jugador és menor que la inicial, busquem menjar
     if(citizen(cell(p).id).type == Builder and citizen(cell(p).id).life < builder_ini_life()) {
              return food_search(p);
     } 
     //Retornem la posició on no hi hagi cap jugador i sigui carrer, si es de nit comprovem si hi ha enemics al voltant.
     else {
        for(Dir d : dirs) {
            if(is_day()) {
               if(pos_ok(p+d) and cell(p+d).id == -1 and cell(p+d).type == Street and  not best_weapons() and cell(p+d).weapon == NoWeapon) return d;
               else if(pos_ok(p+d) and cell(p+d).id == -1 and cell(p+d).type == Street and best_weapons()) return d;
            }
            else {
                if(pos_ok(p+d) and not near_enem(p) and cell(p+d).id == -1 and cell(p+d).type == Street) return d;
            }
        }
     }

     //Com a ultima opció, retornem una direcció random entre {Down, Up, Right, Left}
     return dirs[random(0,3)];
  }

  Dir weapon_search(Pos p) {
    queue<Pos> cua;
    cua.push(p);
    dist_weapon[p.i][p.j] = 0; //la distancia on està el nostre jugador la posem a 0

    //mentre no estigui buida la cua
    while(not cua.empty()) {
        Pos aux = cua.front();
        cua.pop();

        if(pos_ok(aux) and cell(aux).id == -1 and ((cell(aux).weapon == Gun and citizen(cell(p).id).weapon != Gun) or ( citizen(cell(p).id).weapon != Bazooka and cell(aux).weapon == Bazooka))) { //si hem trobat una arma que sigui millor que la del nostre jugador
            while(dist_weapon[aux.i][aux.j] != 0) { //mentre la distància sigui diferent a 0, no hem trobat la posició inicial on estem
                     /* 
                    Si en una posició adjacent trobem que està a menys distancia de la posició aux, actualitzem aux.
                    Arribarem al final quan trobem la posició inicial en la que està el jugador, la posició referent 
                    amb distància igual a 0.
                    */
                    for(Dir d : dirs) {
                        Pos aux_2 = aux + d;
                        if(pos_ok(aux_2) and cell(aux_2).type == Street and (dist_weapon[aux_2.i][aux_2.j]+1) == dist_weapon[aux.i][aux.j]) aux = aux_2;
                        if(dist_weapon[aux.i][aux.j] == 0) {
                            if(d == Down)  return dirs[1];
                            if(d == Up)    return dirs[0];
                            if(d == Left)  return dirs[2];
                            if(d == Right) return dirs[3];
                        }
                    }
            }
        }
        else {
            /*
            Mirem les posicions adjacents a aux, si alguna te valor inf, actualitzem la distancia amb
            distancia(aux)+1 i afegim la posicio a la cua.
            */
            for(Dir d : dirs) {
                if(pos_ok(aux+d) and cell(aux+d).id == -1 and cell(aux+d).type == Street) {
                    Pos next = aux+d;
                    if(dist_weapon[next.i][next.j] == inf) {
                      cua.push(next);
                      dist_weapon[next.i][next.j] = dist_weapon[aux.i][aux.j] + 1;
                    }
                }
            }
        }
    }
      //Si la vida que té el nostre jugador és menor que la inicial, busquem menjar
     if(citizen(cell(p).id).type == Warrior and citizen(cell(p).id).life < warrior_ini_life()) {
              return food_search(p);
     } 
     //Retornem la posició on no hi hagi cap jugador i sigui carrer.
     else {
           for(Dir d : dirs) {
            if(pos_ok(p+d) and cell(p+d).type == Street) {
                Pos x = p+d;
                if(pos_ok(x+d) and cell(x+d).type == Building) {
                    return dirs[random(0,3)];
                }
                else return d;
            }
            }
     }
      //Com a ultima opció, retornem una direcció random entre {Down, Up, Right, Left}
     return dirs[random(0,3)];
  }
  Dir food_search(Pos p) {
    queue<Pos> cua;
    cua.push(p);
    dist_food[p.i][p.j] = 0; //la distancia on està el nostre jugador la posem a 0.


    //mentre la cua no sigui buida
    while(not cua.empty()) {
        Pos aux = cua.front();
        cua.pop();

        if(pos_ok(aux) and cell(aux).id == -1 and cell(aux).bonus == Food) { //si hem trobat el bonus Food
            while(dist_food[aux.i][aux.j] != 0) {
                     /* 
                    Si en una posició adjacent trobem que està a menys distancia de la posició aux, actualitzem aux.
                    Arribarem al final quan trobem la posició inicial en la que està el jugador, la posició referent 
                    amb distància igual a 0.
                    */
                    for(Dir d : dirs) {
                        Pos aux_2 = aux + d;
                        if(pos_ok(aux_2) and cell(aux_2).type == Street and (dist_food[aux_2.i][aux_2.j]+1) == dist_food[aux.i][aux.j]) aux = aux_2;
                        if(dist_food[aux.i][aux.j] == 0) {
                             if(d == Down)  return dirs[1];
                             if(d == Up)    return dirs[0];
                             if(d == Left)  return dirs[2];
                             if(d == Right) return dirs[3];
                        }
                    }
            }
        }
        else {
            /*
            Mirem les posicions adjacents a aux, si alguna te valor inf, actualitzem la distancia amb
            distancia(aux)+1 i afegim la posicio a la cua.
            */
            for(Dir d : dirs) {
                if(pos_ok(aux+d) and cell(aux+d).id == -1 and cell(aux+d).type == Street) {
                    Pos next = aux+d;
                    if(dist_food[next.i][next.j] == inf) {
                      cua.push(next);
                      dist_food[next.i][next.j] = dist_food[aux.i][aux.j] + 1;
                    }
                }
            }
        }
    }
     //Retornem la posició on no hi hagi cap jugador i hagi carrer.
       for(Dir d : dirs) {
            if(pos_ok(p+d) and cell(p+d).type == Street) {
                Pos x = p+d;
                if(pos_ok(x+d) and not near_enem(x) and cell(x+d).type == Building) {
                    return dirs[random(0,3)];
                }
                else return d;
            }
    }
     //Com a ultima opció, retornem una direcció random entre {Down, Up, Right, Left}
     return dirs[random(0,3)];
  }

  Dir enemies_search(Pos p) {
    queue<Pos> cua;
    cua.push(p);
    dist_enemies[p.i][p.j] = 0; //la distancia on està el nostre jugador la posem a 0.


    //mentre la cua no sigui buida
    while(not cua.empty()) {
        Pos aux = cua.front();
        cua.pop();

        if (pos_ok(aux) and cell(aux).id != -1 and citizen(cell(aux).id).player != me() and ((weapon_strength_attack(citizen(cell(p).id).weapon) > weapon_strength_attack(citizen(cell(aux).id).weapon)) or (cell(aux).b_owner != -1 and cell(aux).resistance != -1))) { //si hem trobat un constructor o un enemic rival amb una arma inferior a la del nostre jugador
            while(dist_enemies[aux.i][aux.j] != 0) {    
                    /* 
                    Si en una posició adjacent trobem que està a menys distancia de la posició aux, actualitzem aux.
                    Arribarem al final quan trobem la posició inicial en la que està el jugador, la posició referent 
                    amb distància igual a 0.
                    */
                    for(Dir d : dirs) {
                        Pos aux_2 = aux + d;
                        if(pos_ok(aux_2) and cell(aux_2).type == Street and (dist_enemies[aux_2.i][aux_2.j]+1) == dist_enemies[aux.i][aux.j]) aux = aux_2;
                        if(dist_enemies[aux.i][aux.j] == 0) {
                            if(d == Down)  return dirs[1];
                            if(d == Up)    return dirs[0];
                            if(d == Left)  return dirs[2];
                            if(d == Right) return dirs[3];
                        }
                    }
            }
        }
        else {
            /*
            Mirem les posicions adjacents a aux, si alguna te valor inf, actualitzem la distancia amb
            distancia(aux)+1 i afegim la posicio a la cua.
            */
            for(Dir d : dirs) {
                if(pos_ok(aux+d) and cell(aux+d).type == Street) {
                    Pos next = aux+d;
                    if(dist_enemies[next.i][next.j] == inf) {
                      cua.push(next);
                      dist_enemies[next.i][next.j] = dist_enemies[aux.i][aux.j] + 1;
                    }
                }
            }
        }
    }

     //Retornem la posició on hi hagi carrer, si a la seguent posició hi ha edifici, retornem posició random per no produir un bloqueig al jugador.
    for(Dir d : dirs) {
            if(pos_ok(p+d) and cell(p+d).type == Street) {
                Pos x = p+d;
                if(pos_ok(x+d) and cell(x+d).type == Building) {
                    return dirs[random(0,3)];
                }
                else return d;
            }
    }
    //Com a ultima opció, retornem una direcció random entre {Down, Up, Right, Left}
    return dirs[random(0,3)];
 }



  virtual void play () {
    bld = builders(me());
    war = warriors(me());

    dist_money =   MatrixD(board_rows(), Row(board_cols(), inf));
    dist_enemies =  MatrixD(board_rows(), Row(board_cols(), inf));
    dist_food =     MatrixD(board_rows(), Row(board_cols(), inf));
    dist_weapon =  MatrixD(board_rows(), Row(board_cols(), inf));
    dist_moneyw = MatrixD(board_rows(), Row(board_cols(), inf));

    def = barricades(me());

    if(is_day()) {
        for(int id : bld) {
            if(citizen(id).life > 0) {
                Pos pbld = citizen(id).pos;
                Dir x = money_search(pbld);
                move(id, x);
            }
        }
        for(int id2 : war) {
            if(citizen(id2).life > 0){
                Pos pwar = citizen(id2).pos;
                if(citizen(id2).weapon != Bazooka) {
                    Dir y = weapon_search(pwar);
                    move(id2, y);
                }
                else if(is_day() and num_rounds()%num_rounds_per_day() == 23) {
                     Dir m = enemies_search(pwar);
                     move(id2, m);
                }
                else {
                    Dir z = moneyw_search(pwar);
                    move(id2, z);
                }
            }
        }
    }
    else {
        for (int id : war) {
            if(citizen(id).life > 0) {
                Pos pwar = citizen(id).pos;
                Dir x = enemies_search(pwar);
                move(id, x);
            }
        }
        for(int id2 : bld) {
            if(citizen(id2).life > 0) {
                Pos pbld = citizen(id2).pos;
                Dir y = money_search(pbld);
                move(id2, y);
            }
        }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);