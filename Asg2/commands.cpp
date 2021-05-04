#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
   {"#"     , fn_com   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such command");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_com (inode_state& state, const wordvec& words){
   return;
}

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size() == 1){
      return;
   }

   string filename = words[1];

   if(state.getCwd()->getContent()->getPtr(filename) != nullptr){
      
      wordvec data = state.getCwd()->getContent()->
      getPtr(filename)->getContent()->readfile();
      for(int i = 0; i < data.size(); i++){
         cout << data[i] << " ";
      }
      cout << endl;
   }
   else{
      cout << "cat: " << filename << ": No such file or directory";
   }

}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   // Just cd check
   if(words.size() == 1){
      state.setCwd(state.getRoot());
      return;
   }

   wordvec splitWords = split(words[1], "/");

   for(int i = 0; i < splitWords.size(); i++){
      if(state.getCwd()->getContent()->
      getPtr(splitWords[i]) == nullptr){
         cout << "WORD:" << splitWords[i] << endl;
         cout << "Error: No such directory!!!!!" << endl;
      }
      else{
         inode_ptr curr = state.getCwd()->
         getContent()->getPtr(splitWords[i]);
         state.setCwd(curr);
      }
   }
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr cD = state.getCwd();
   inode_ptr temp = state.getCwd();
   int size = cD->getContent()->getDirent().size();
   map<string, inode_ptr>::iterator it;

   // If we want to ls a remote directory,
   // print relative path to directory
   if(words.size() == 2){
      // case for if they do "ls /"
      if(words[1] == "/"){
         cD = state.getRoot();
         for(it = cD->getContent()->getDirent().begin(); 
         it != cD->getContent()->getDirent().end(); it++){

         cout << it->second->get_inode_nr() <<
               "        " << it->first << endl;
         }
         return;
      }

      if(state.getCwd()->getContent()->getPtr(words[1])->getContent()->getError() == "plain file"){
          cout << "Error: Non-Directory Type!" << endl;
          return;
      }

      fn_cd(state, words);

      string name = "";
      while(state.getCwd() != cD){
         name = "/" + state.getCwd()->getName() + name;
         wordvec goParent;
         goParent.push_back(words[0]); 
         goParent.push_back("..");
         fn_cd(state, goParent);
      }

      cout << name << ":" << endl;
      fn_cd(state, words);
      cD = state.getCwd();
   }
   
   for(it = cD->getContent()->getDirent().begin(); 
      it != cD->getContent()->getDirent().end(); it++){

      cout << it->second->get_inode_nr() <<
              "        " << it->first << endl;
   }
   state.setCwd(temp);
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr keep = state.getCwd();
   wordvec path;
   path.push_back(words[0]);
   
   if(words.size() == 2){
      path.push_back(words[1]);
      fn_cd(state, path);
      path.pop_back();
   }

   fn_pwdLsr(state, path);
   fn_ls(state, path);

   for( const auto &pair : state.getCwd()->getContent()->getDirent()){
      if(pair.first != "." and pair.first != ".."){
         if(state.getCwd()->getContent()->getError() != "plain file"){
            wordvec temp;
            temp.push_back(path[0]);
            temp.push_back(pair.first);
            fn_lsr(state, temp);

         }
      }
   }
   state.setCwd(keep);
}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   string filename = words[1];
   if(state.getCwd()->getContent()->getPtr(filename) == nullptr){
      cout << "in make" << endl;
      
      wordvec contents;
      int contents_size = 0;
      for(int i = 2; i < words.size(); i++){
         contents_size += words[i].size();
         contents.push_back(words[i]);
      }

      // cout << "File contents: " << contents << endl;
      // cout << "Contents size: " << contents_size << endl;
      inode_ptr newFile = state.getCwd()->getContent()
      ->mkfile(filename);
      newFile->getContent()->writefile(contents);
   }
   else{
      cout << "Error: File Already Exists" << endl; 
   }

}

bool checkIn(inode_state& state, const wordvec& words){
   if(words.size() == 1){
      return true;
   }
   wordvec splitWords = split(words[1], "/");
   inode_ptr save_curr = state.getCwd();

   for(int i = 0; i < splitWords.size(); i++){
      wordvec temp;
      temp.push_back(words[0]);
      temp.push_back(splitWords[i]);
      if(state.getCwd()->getContent()->
      getPtr(splitWords[i]) == nullptr){
         state.setCwd(save_curr);
         return false;
      }
      else{
         fn_cd(state, temp);
      }
   }
   state.setCwd(save_curr);
   return true;
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr saveCurr = state.getCwd();
   string temp = words[1];

   if(words.size() > 2){
      cout << "Error: Too many arguments." << endl;
      return;
   }

   wordvec s = split(words[1], "/");
   //cout << s << endl;
   if(s.size() > 1){
      while(temp.back() != '/'){
         temp.pop_back();
      }
      temp.pop_back(); // remove "/"
      //cout << "Temp: " << temp << endl;
      wordvec path;
      path.push_back(words[0]);
      path.push_back(temp);
      if(checkIn(state, path) == true){
         fn_cd(state, path);
      }
      else{
         cout << "Error, Directory does not exist: " << path[1] << endl;
         return;
      }
   }

   wordvec spl = split(words[1], "/");
   string word = spl[spl.size() - 1];
   if(state.getCwd()->getContent()->getPtr(word) == nullptr){
      state.getCwd()->getContent()->mkdir(word);
      inode_ptr newDir = state.getCwd()->getContent()->getPtr(word);
      newDir->getContent()->getDirent()[".."] = state.getCwd();
   }
   else{
      cout << "Error: Directory Already Exists" << endl; 
   }
   state.setCwd(saveCurr);
}



void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   state.prompt(words);
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(state.getCwd() == state.getRoot()){
      cout << "/" << endl;
      return;
   }
   inode_ptr save = state.getCwd();
   wordvec goParent;
   goParent.push_back(words[0]); 
   goParent.push_back("..");
   string name = "";
   while(state.getCwd() != state.getRoot()){
      name = "/" + state.getCwd()->getName() + name;
      fn_cd(state, goParent);
   }
   state.setCwd(save);
   cout << name << endl;
   return;
}

void fn_pwdLsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(state.getCwd() == state.getRoot()){
      cout << "/:" << endl;
      return;
   }
   inode_ptr save = state.getCwd();
   wordvec goParent;
   goParent.push_back(words[0]); 
   goParent.push_back("..");
   string name = "";
   while(state.getCwd() != state.getRoot()){
      name = "/" + state.getCwd()->getName() + name;
      fn_cd(state, goParent);
   }
   state.setCwd(save);
   cout << name << ":" << endl;
   return;
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr keep = state.getCwd();
   string temp = words[1];


   wordvec s = split(words[1], "/");
   if(s.size() > 1){
      while(temp.back() != '/'){
         temp.pop_back();
      }
      temp.pop_back(); // remove "/"
      wordvec path;
      path.push_back(words[0]);
      path.push_back(temp);
      //cout << "Before checkin: " << path[1] << endl;
      if(checkIn(state, path) == true){
         fn_cd(state, path);
         wordvec spl = split(words[1], "/");
         string word = spl[spl.size() - 1];
         if(state.getCwd()->getContent()->getPtr(word) != nullptr){
            state.getCwd()->getContent()->remove(word);
         }
         state.setCwd(keep);
         return;
      }
      else{
         cout << "Error, Directory does not exist: " << path[1] << endl;
         return;
      }
   }
   if(state.getCwd()->getContent()->getPtr(words[1]) != nullptr){
      state.getCwd()->getContent()->remove(words[1]);
   }

}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}