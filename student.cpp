<<<<<<< HEAD
#define _CRT_SECURE_NO_WARNINGS
# include "student.h"
void enterStruct(stud sd[], int kol){
for(int i=0; i < kol; i++){
cout << "\nПрізвище: ";cin >> sd[i].surname;
cout << "Им'я: "; cin >> sd[i].name;
cout << "По батькові: "; cin >> sd[i].name2;
cout << "Група: "; cin >> sd[i].grupa; }
}
void fileWrite(char* txt,stud sd[], int kol) {
ofstream t(txt,ios::binary | ios::app);
for(int i=0;i<kol;i++){
t.write((char*)&sd[i], sizeof sd[i]); }
t.close(); }
void printStruct(stud* sd,int kol){
for(int i = 0; i < kol; i++){
cout << "\nПрізвище: "<< sd[i].surname;
cout << "\nИм'я: "<< sd[i].name;
cout << "\nПо батькові: "<<sd[i].name2;
cout << "\nГрупа: "<< sd[i].grupa<<"\n";}}
int fileOfSize (char *txt) {
ifstream t(txt,ios::binary);
if (!t.is_open()){
cout << "Файл не може бути відкритий !\n";return 0; }
t.seekg(0,ios_base::end);
int k = t.tellg()/sizeof (stud);
t.close();
return k;}
void readFile(char *txt,stud* sd,int kol) {
ifstream t(txt,ios::binary );
for(int i=0;i<kol;i++){
t.read((char*)&sd[i], sizeof sd[i]); }
t.close();}
void rewriteFile(char* txt,stud* sd,int kol) {
ofstream t(txt,ios::binary);
for(int i=0;i<kol;i++){
t.write((char*)&sd[i], sizeof sd[i]);}
t.close();}
void sortStud(stud* sd,int kol){
stud s;
for(int i=1;i<kol;i++)
for(int j=kol-1;j>=i;j--)
if(stricmp(sd[j].surname, sd[j-1].surname) < 0){
s = sd[j];
sd[j]=sd[j-1];
sd[j-1]=s;
}
else if(stricmp(sd[j].surname, sd[j-1].surname) ==0){
if(stricmp(sd[j].name, sd[j-1].name) < 0){
s = sd[j];
sd[j]=sd[j-1];
sd[j-1]=s;
}
}}
=======
#define _CRT_SECURE_NO_WARNINGS
# include "student.h"
void enterStruct(stud sd[], int kol){
for(int i=0; i < kol; i++){
cout << "\nПрізвище: ";cin >> sd[i].surname;
cout << "Им'я: "; cin >> sd[i].name;
cout << "По батькові: "; cin >> sd[i].name2;
cout << "Група: "; cin >> sd[i].grupa; }
}
void fileWrite(char* txt,stud sd[], int kol) {
ofstream t(txt,ios::binary | ios::app);
for(int i=0;i<kol;i++){
t.write((char*)&sd[i], sizeof sd[i]); }
t.close(); }
void printStruct(stud* sd,int kol){
for(int i = 0; i < kol; i++){
cout << "\nПрізвище: "<< sd[i].surname;
cout << "\nИм'я: "<< sd[i].name;
cout << "\nПо батькові: "<<sd[i].name2;
cout << "\nГрупа: "<< sd[i].grupa<<"\n";}}
int fileOfSize (char *txt) {
ifstream t(txt,ios::binary);
if (!t.is_open()){
cout << "Файл не може бути відкритий !\n";return 0; }
t.seekg(0,ios_base::end);
int k = t.tellg()/sizeof (stud);
t.close();
return k;}
void readFile(char *txt,stud* sd,int kol) {
ifstream t(txt,ios::binary );
for(int i=0;i<kol;i++){
t.read((char*)&sd[i], sizeof sd[i]); }
t.close();}
void rewriteFile(char* txt,stud* sd,int kol) {
ofstream t(txt,ios::binary);
for(int i=0;i<kol;i++){
t.write((char*)&sd[i], sizeof sd[i]);}
t.close();}
void sortStud(stud* sd,int kol){
stud s;
for(int i=1;i<kol;i++)
for(int j=kol-1;j>=i;j--)
if(stricmp(sd[j].surname, sd[j-1].surname) < 0){
s = sd[j];
sd[j]=sd[j-1];
sd[j-1]=s;
}
else if(stricmp(sd[j].surname, sd[j-1].surname) ==0){
if(stricmp(sd[j].name, sd[j-1].name) < 0){
s = sd[j];
sd[j]=sd[j-1];
sd[j-1]=s;
}
}}
>>>>>>> 4f341bf29cd8b1e1111d5b7620c9a622be496f25
