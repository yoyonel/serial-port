
#include "AsyncSerial.h"

#include <iostream>
#include <termios.h>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

using namespace std;

/**
 * @brief received callback de reception de donnees
 * via la serial connection
 * @param data tableau de donnees const char*
 * @param len longueur du tableau (i.e. nombre de donnees recues)
 */
void received(const char *data, unsigned int len)
{
    const std::string string_serial_data(data, len);

    cout << string_serial_data; // gere la couleur automatiquement (retour de ligne) :-)

    cout.flush();   // Flush screen buffer
}

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        cerr<<"Usage: serial port baudrate"<<endl<<
              "To quit type Ctrl-C x"<<endl<<
              "To send Ctrl-C type Ctrl-C Ctrl-C"<<endl;
        return 1;
    }

    // -------------------------
    // configuration du terminal
    // -------------------------
    // sauvegarde des settings du terminal
    // courant
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    // mise en place d'un nouveau settings
    termios new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
    new_settings.c_lflag &= ~(ECHO); // no echo
    tcsetattr(0, TCSANOW, &new_settings);
    // -------------------------

    cout<<"\e[2J\e[1;1H"; //Clear screen and put cursor to 1;1

    try {
        // -------------------------
        // Mise en place de la connection serie
        // - Recuperation des arguments: adress + port
        // - Mise en place du callback de reception des donnees.
        CallbackAsyncSerial serial(argv[1],
                boost::lexical_cast<unsigned int>(argv[2]));
        serial.setCallback(received);
        // -------------------------

        // -------------------------
        // Loop de communication
        // via le serial connection
        // -------------------------
        bool b_continue = true;
        while(b_continue)
        {
            if(serial.errorStatus() || serial.isOpen()==false)
            {
                cerr<<"Error: serial port unexpectedly closed"<<endl;
                break;
            }
            char c;
            cin.get(c); //blocking wait for standard input

            // -------------------------
            // Gestion de l'interruption break
            // -------------------------
            if(c==3) //if Ctrl-C
            {
                cin.get(c);
                switch(c)
                {
                case 3:
                    serial.write(&c,1);//Ctrl-C + Ctrl-C, send Ctrl-C
                    break;
                case 'x': //fall-through
                case 'X':
                    //Ctrl-C + x, quit
                    b_continue = false;
                    break;
                case 'l':
                case 'L':
                    // simule 'ls' shortcut
                    serial.writeString(std::string("ls /img/*.tif\n"));
                    break;
                default:
                    serial.write(&c,1);//Ctrl-C + any other char, ignore
                }
            }
            else {
                // -------------------------
                // Sinon on envoie caractere par caractere
                // a la connection serial
                // -------------------------
                serial.write(&c,1);
            }
        }
        // -------------------------
        // Fermeture de la connection serial
        // -------------------------
        serial.close();
        // -------------------------
    } catch (std::exception& e) {
        cerr<<"Exception: "<<e.what()<<endl;
    }

    // -------------------------
    // restauration des settings du terminal
    // -------------------------
    tcsetattr(0, TCSANOW, &stored_settings);
    // -------------------------
}
