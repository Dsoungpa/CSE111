#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "debug.h"
#include "file_sys.h"

size_t inode::next_inode_nr {1};

ostream& operator<< (ostream& out, file_type type) {
   switch (type) {
      case file_type::PLAIN_TYPE: out << "PLAIN_TYPE"; break;
      case file_type::DIRECTORY_TYPE: out << "DIRECTORY_TYPE"; break;
      default: assert (false);
   };
   return out;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
   root = make_shared<inode>(file_type::DIRECTORY_TYPE);
   cwd = root;
   root->getContent()->addMap(".", root);
   root->getContent()->addMap("..", root);
   root->getContent()->setAm(2);
   root->setName("/");
}

void inode_state::prompt(const wordvec& words){
   this->prompt_ = "";
   for(int i = 1; i < words.size(); i++){
      this->prompt_ += words[i] + " ";
   }
}

inode_ptr inode_state::getCwd(){
   return this->cwd;
}

inode_ptr inode_state::getRoot(){
   return this->root;
}

void inode_state::setCwd(inode_ptr curr){
   this->cwd = curr;
}

const string& inode_state::prompt() const { return prompt_; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
      default: assert (false);
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

size_t inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

void inode::setName(const string& dirname){
   this->name = dirname;
}

string inode::getName(){
   return this->name;
}

base_file_ptr inode::getContent(){
   return this->contents;
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

const string& base_file::getError(){
   return this->error_file_type();
}

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}
void base_file::addMap(const string& dirname, inode_ptr ptr){
   throw file_error ("is a " + error_file_type());
}
inode_ptr base_file::getPtr(const string& dirname){
   throw file_error ("is a " + error_file_type());
}
 map<string, inode_ptr>& base_file::getDirent(){
   throw file_error ("is a " + error_file_type());
 }
size_t base_file::get_number(){
   throw file_error ("is a " + error_file_type());
 }

 void base_file::setAm(int num){
   throw file_error ("is a " + error_file_type());
}

int base_file::getAm(){
   throw file_error ("is a " + error_file_type());
}





void plain_file::setAm(int num){
   this->amount = num;
}

int plain_file::getAm(){
   return this->amount;
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   this->data = words;
}

size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::addMap(const string& dirname, inode_ptr ptr){
   if(this->dirents.find(dirname) == this->dirents.end()){
      this->dirents[dirname] = ptr;
   }
   else{
      cout << "Error: Directory already exists" << endl;
   }
}


void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   if(this->getPtr(filename)->getContent()->getError() == "plain file"){
      //cout << "in plain erase" << endl;
      this->getDirent().erase(filename);
      this->setAm(this->getAm() - 1);
   }
   else{
      if(this->getPtr(filename)->getContent()->getDirent().size() == 2){
         //cout << "in dir erase" << endl;
         this->getDirent().erase(filename);
         this->setAm(this->getAm() - 1);
      }
      else{
         cout << "Error: Non-empty Directory!" << endl;
      }
   }

}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr newFile = make_shared<inode>(file_type::PLAIN_TYPE);
   newFile->setName(filename);
   this->getDirent()[filename] = newFile;
   return newFile;
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   inode_ptr newDir = make_shared<inode>(file_type::DIRECTORY_TYPE);
   newDir->getContent()->getDirent()["."] = newDir;
   newDir->setName(dirname);
   this->getDirent()[dirname] = newDir;
   newDir->getContent()->setAm(2);
   this->setAm(this->getAm()+1);
   //cout << "Got to mkdir" << endl;
   return newDir;
}

 map<string, inode_ptr>& directory::getDirent(){
    return this->dirents;
 }

inode_ptr directory::getPtr(const string& dirname){
   if(this->dirents.find(dirname) == dirents.end()){
      return nullptr;
   }
   return dirents[dirname];
}

size_t directory::get_number(){
   //return this->inode_nr;
 }

void directory::setAm(int num){
   this->amount = num;
}

int directory::getAm(){
   return this->amount;
}
