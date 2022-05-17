#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std ;

void loginOrCreate() ;
void createAccount();
void changePassword( string username , bool key ) ;
bool isLoggedIn( int cnt ) ;
int generateKey() ;
void encrypt( int key , string email , string username , string password , string country , string address , string phoneNumber ) ;
void decrypt( string username , int key , int mode ) ;
void clearScreen() ;

void createAccount() {
    cout << "Perfect, let's create your brand new account!\n" ;
    string email , username , password , passwordVerification , country , phoneNumber , address ;
    tag :
    cout << "E-mail : " ; cin >> email ;
    if( email.find("@") > email.length() || email.find("@") == 0 ) {
        cout << "Invalid e-mail address. Try again!\n" ;
        goto tag ;
    }
    cout << "Username : " ; cin >> username ;
    jump :
    cout << "Password: " ; cin >> password ;
    cout << "Got it, now let's verify the password!\n" ; cin >> passwordVerification ;
    if( password == passwordVerification ) {
        ofstream print("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
        cout << "Passwords match.\nTell us the city you live in!\n" ; cin >> address ;
        cout << "I think we forgot to mention... what country do you live in?\n" ; cin >> country ;
        cout << "We need your phone number ( without the prefix ) and you are all set.\n" ;
        boom :
        cout << "Enter your phone number : " ; cin >> phoneNumber ;
        if( phoneNumber.length() != 10 ) {
            cout << "Invalid phone number. Try again!\n" ;
            goto boom ;
        }
        print << email << "\n" << username << "\n" << password << "\n" << country << "\n" << address << "\n" << phoneNumber ;
        print.close() ;
    }else {
        cout << "Passwords don't match... try again!\n" ;
        goto jump ;
    }
    int key = generateKey() ;
    encrypt( key , email , username , password , country , address , phoneNumber );
    cout << "Account successfully created! Now you can finally login if you'd like to.\nDo you want to login?\n" ;
    string choice ;
    cin >> choice ;
    if( choice[0] == 'Y' || choice[0] =='y' ) {
        clearScreen() ;
        isLoggedIn(0);
    }
}
void changePassword( string username , bool key ) {
    string  unusedUsername , unusedPassword , country , address , phoneNumber ;
    if( key == false ) {
        cout << "Before we can start, you need to enter your email to be sure that it's really you.\n" ;
        string email , validEmail ;
        ifstream read("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
        read >> validEmail >> unusedUsername >> unusedPassword >> country >> address >> phoneNumber ;
        read.close() ;
        again :
        cout << "Email : " ;
        cin >> email ;
        if( validEmail == email )
            cout << "Okay, let's start with your new password!\n";
        else goto again ;
    }
    string password , passwordVerification ;
    damn :
    cout << "What would you want to be your next password?\n" ; cin >> password ;
    cout << "Got it, now let's verify the password!\n" ; cin >> passwordVerification ;
    if( password == passwordVerification ) {
        ofstream print("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
        print << username << "\n" << password << "\n" << country << "\n" << address << "\n" << phoneNumber ;
        print.close() ;
        cout << "Password succesfully changed!\nReady to login!\n" ;
    }else {
        cout << "Passwords don't match... try again!\n" ;
        goto damn ;
    }
    string choice ;
    cout << "Do you want to login?\n" ; cin >> choice ;
    if( choice[0] == 'Y' || choice[0] =='y' )
        isLoggedIn(0) ;
}

bool isLoggedIn( int cnt ) {
    int key ;
    string email , username , password , user , pass , country , address , phoneNumber ;
    cout << "Username: " ; cin >> username ;
    cout << "Password: " ; cin >> password ;
    cout << "Decrypt key: " ; cin >> key ;
    ifstream read("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
    decrypt(username , key , 0) ;
    getline(read , email) ;
    getline(read , user) ;
    getline(read , pass) ;
    getline(read , country) ;
    getline(read , address) ;
    getline(read , phoneNumber) ;
    encrypt(key , email , username , password , country , address , phoneNumber) ;
    if( username == user  && password == pass ) {
        cout << "Logged in successfully!\n" ;
        string choice ;
        cout << "If you want, you can see your all info!\n" ; cin >> choice ;
        if( choice[0] == 'y' || choice[0] == 'Y' ) {
            int key ;
            cout << "Enter your decrypt key : " ; cin >> key ;
            decrypt(username , key , 1);
        }
        encrypt(key , email , username , password , country , address , phoneNumber) ;
        return true ;
    }else if( username == user && password != pass ) {
        cout << "Existing account, wrong password.\n" ;
        if(cnt >= 2) {
            string choice ;
            cout << "Do you want to change the password?\n" ; cin >> choice ;
            if( choice[0] == 'y' || choice[0] == 'Y' )
                changePassword(username , false) ;
            else isLoggedIn(cnt + 1) ;
        }else isLoggedIn(cnt + 1) ;
    }else {
        cout << "Couldn't find any account based on this username...\nDo you want to create one?\n" ;
        string choice ;
        cin >> choice ;
        if( choice[0] == 'y' || choice[0] == 'Y' )
            createAccount() ;
        return false ;
    }
    read.close() ;
}
void loginOrCreate() {
    bool log ;
    cout << "Hello and welcome to Levi's small database.\nDo you have an account and want to login or do you wanna create one?\n" ;
    char choice[20] ;
    cin >> choice ;
    for( int i = 0 ; i < strlen(choice) ; i++ )
        choice[i] = tolower(choice[i]) ;
    if( strstr(choice , "login") )
        isLoggedIn(0) ;
    else if( strstr(choice , "register") || strstr(choice , "create") )
        createAccount() ;
}

int generateKey() {
    srand(time(NULL)) ;
    int key = (rand() % 10) + 1 ;
    cout << "Your key to your data is : " << key <<"! Do not forget it, otherwise you'll lose all your data!\nOur tip is to write it down.\n" ;
    return key ;
}
void encrypt( int key , string email , string username , string password , string country , string address , string phoneNumber ) {
    ofstream print("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;

    for( int i = 0 ; i < email.length() ; i++ )
        email[i] += key ;
    if( key < 9 )
        key++ ;
    else key-- ;
    for( int i = 0 ; i < username.length() ; i++ )
        username[i] += key ;
    if( key < 9 )
        key++ ;
    else key-- ;
    for( int i = 0 ; i < password.length() ; i++ )
        password[i] += key ;
    if( key > 1 )
        key-- ;
    else key++ ;
    for( int i = 0 ; i < country.length() ; i++ )
        country[i] += key ;
    if( key < 9 )
        key++ ;
    else key-- ;
    for( int i = 0 ; i < address.length() ; i++ )
        address[i] += key ;
    if( key > 1 )
        key-- ;
    else key++ ;
    for( int i = 0 ; i < phoneNumber.length() ; i++ )
        phoneNumber[i] += key ;

    print << email << "\n" << username << "\n" << password << "\n" << country << "\n" << address << "\n" << phoneNumber ;
    print.close() ;
}
void decrypt( string username , int key , int mode ) {
    string unusedUsername , password , email , address , phoneNumber , country ;
    ifstream read("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
    read >> email ; read >> unusedUsername ; read >> password ; read >> country ; read >> address ; read >> phoneNumber ;
    read.close() ;
    for( int i = 0 ; i < email.length() ; i++ )
        email[i] -= key ;
    if( key < 9 )
        key++ ;
    else key-- ;
    if( key < 9 )
        key++ ;
    else key-- ;
    for( int i = 0 ; i < password.length() ; i++ )
        password[i] -= key ;
    if( key > 1 )
        key-- ;
    else key++ ;
    for( int i = 0 ; i < country.length() ; i++ )
        country[i] -= key ;
    if( key < 9 )
        key++ ;
    else key-- ;
    for( int i = 0 ; i < address.length() ; i++ )
        address[i] -= key ;
    if( key > 1 )
        key-- ;
    else key++ ;
    for( int i = 0 ; i < phoneNumber.length() ; i++ )
        phoneNumber[i] -= key ;
    if( mode )
        cout << email << "\n" << username << "\n" << password << "\n" << country << "\n" << address << "\n" << phoneNumber << "\n";
    else {
        ofstream print("/Users/levismac/Documents/Baza_De_Date_CPP/" + username + ".txt") ;
        print << email << "\n" << username << "\n" << password << "\n" << country << "\n" << address << "\n" << phoneNumber << "\n";
    }
}

void clearScreen() {
    for( int i = 0 ; i < 15 ; i++ )
        cout << "\n\n" ;
}
//de facut requirements la parola (cifre, simboluri, litere mici si mari)!

int main() {
    loginOrCreate() ;
    return 0 ;
}
