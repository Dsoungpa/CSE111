// $Id: commands.cpp,v 1.20 2021-01-11 15:52:17-08 - - $

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

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // Print state.contents to cout


}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   inode_ptr temp = state.get_temp();
   map<string, inode_ptr> map = temp->get_map();
   for(int i = 0; i < words.size(); i++){{
      if(map.find(words[i]) == map.end()){
         cout << "Error, no such directory found: " << words[i] << endl;
         state.set_temp();
         return;
      }
      temp = map.find(words[i]);
   }
   state.set_cwd(temp);
   state.set_temp();
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

   inode_ptr temp = state.get_temp();
   map<string, inode_ptr> map = temp.get_map;
 
   string path = words[1];

   words = split_string(words[1]);
 
   for(int i = 0; i < words.size(); i++){{
      map = temp.get_map();
      if(map.find(words[i]) == map.end()){
         cout << "Error, no such directory found: " << words[i] << endl;
         set_temp();
         return;
      }
      
      temp = map.find(words[i]);
   }
   // Print cwd
   cout << path << ":" << endl;
   std::map<string, inode_ptr>::iterator it;
   // Iterate through cwd file list
   for(it = map.begin(); it != map.end(); it++){
   
      // Print unique inode number, file/directory size, and name
      cout << it.first() << "/" << endl;

   }

}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // Call ls on cwd
   // For directory in cwd:
   //    call lsr

}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // If filename in name list, throw error
   

   // add filename to ordered names list
   // create inode, connect it to cwd
   // Add filename: contents to hashmap

}

std::vector<string> split_string (const string& words){

   std::vector<string> new_;
   string temp = "";
   for(int i = 0; i < words.size(); i++){
      //cout << words[i] << endl;
      if(words[i] != '/'){
         temp += words[i];
      }
      else{
         new_.push_back(temp);
         temp = "";
      }
   }
   new_.push_back(temp);
   return new_;
}




void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
 
   // If directory in name list, throw error
   std::vector<string> new_string = split_string(words[1]);

   cout << new_string << endl;

   directory_name = new_string.back();
   new_string.pop_back();

   fn_cd(state, new_string);

   inode_ptr temp = state.get_temp();
   map<string, inode_ptr> map = temp.get_map();

   if(map.find(directory_name) != map.end()){
      cout << "Error: Directory already exists! " << directory_name << endl;
      return;
   }
   else{
      temp.mkdir(directory_name);
   }      
}

void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   //
   state.prompt(words);

}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // print name of cwd
   
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // Use temp cursor to cd to pathname's parent directory
   //
   // If pathname doesn't exist, throw error
   //
   // if pathname is file, remove file name from list, delete from hgashmap, free memory
   //
   // if pathname is directory, check if it's empty
   // if empty directory, remove directory name from list, delete from hashmap, free memory

}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // Use temp cursor to cd to pathname's parent directory
   //
   // if pathname doesn't exist, throw error
   //
   // If pathname is file, remove file name from list, delete from hashmap, free memory
   //
   // if pathname is directory:
   
      // Iterate through directory contents
      // for file/directory in directory:
         // rmr file/directory


}

