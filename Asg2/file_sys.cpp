// $Id: file_sys.cpp,v 1.10 2021-04-10 14:23:40-07 - - $

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

   // Constructor for root directory

   root = make_shared<inode>(file_type::DIRECTORY_TYPE);
   cwd = root;
   root->dirents.insert(std::pair<string, inode_ptr>(".", root));
   root->dirents.insert(std::pair<string, inode_ptr>("..", root));

}

//////////////////////////////////////
///////// GETTER FUNCTIONS ///////////
//////////////////////////////////////

inode_ptr inode_state::get_cwd() { return cwd;}

void inode_state::set_cwd(inode_ptr temp){
   cwd = temp
   return;
}

inode_ptr inode_state::get_root() {return root;}

inode_ptr inode_state::get_temp() {return temp;}

void inode_state::set_temp(){ temp = cwd; return;}

map<string, inode_ptr> directory::get_map() {return dirents;}


/////////////////////////////////////


const string& inode_state::prompt() const { return prompt_; }

static inode_ptr inode::parent(){ return parent; }

void inode_state::prompt (const wordvec& words){

   this->prompt_ = "";
   for(int i = 1; i < words.size(); i++){
   
      this->prompt_ += words[i] + " ";
   
   }
}


void inode_state::cdp (const string& word){

//   this->cwd = this->cwd->parent;
}


void inode_state::ls (){
/*
   cout << this->cwd->name << ":"<< endl;
   

   map<string, inode_ptr>::iterator it;
   for(it = this->cwd->dirents.begin(); it != this->cwd->dirents.end(); it++){

      int inode_num = it->second.inode_nr;
      int size = it->second.size();
      string name = it->first;

      

   }*/

}
      


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


file_error::file_error (const string& what):
            runtime_error (what) {
}

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());

   // If file
      // remove from hashmap/dictionary
      // decrement directory size by 1
      // delete file (and contents if needed)
      // free memory
   
   // If Directory
      // Iterate through directory
      // if sub-directory:
         // Call remove on sub-directory and all of it's contents
      // if file:
         // Call remove on file
      // free memory

}
/*
std::vector<string> split_string (const string& words){

   std::vector<string> new_;
   string temp = "";
   for(int i = 1; i < words.size(); i++){
      cout << words[i] << endl;

      if(words[i] != "/"){
         temp += words[i];
      }
      else{
         new_.push_back(temp);
         temp = "";
      }
   }
   return new_;
} */  



inode_ptr base_file::mkdir (const wordvec& words) {
   throw file_error ("is a " + error_file_type());

   // for every word in vector-1:
 //  for(int i = 0; i < words.size(); i++){

      // If word in cwd map:
   //   if(this->dirents.find(words[i]) != this->dirents.end()){ 
         // temp cd to corresponding directory
         
      // if word not in cwd map:
      //    throw error
   
   // In parent directory
   // construct directory 
   // add it to parent directory list/map
   // add parent directory to new directory
   // new.set(parent, new)

}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}


size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);

   // Go through file, count characters

   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
}

void directory::set (inode_ptr p, inode_ptr s){

   this->parent = p;
   this->self = s;
   this->dirents.insert({".", s});
   this->dirents.insert({"..", p});
}

size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   return nullptr;

   inode_ptr new_ = make_shared<DIRECTORY_TYPE>();

   new_.set(this, new_); 
   
   this->dirents.insert({dirname, new_});
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   return nullptr;
}

