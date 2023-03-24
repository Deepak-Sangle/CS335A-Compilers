#include "bits/stdc++.h"
#include "inc/symtab.h"
#include <string.h>

using namespace std;

extern void yyerror(char const*);
extern GlobalSymbolTable* global_symtab;

void GlobalSymbolTable::increase_level(){
    pair<int,int> curr_level = level_stack.top();
    int new_main_level = curr_level.first + 1;
    int new_sub_level = (symbol_tables.size() <= new_main_level) ? 0 : symbol_tables[new_main_level].size();
    pair<int,int> new_level = {new_main_level, new_sub_level};
    level_stack.push(new_level);
    // creating sybmol table entry for new_level;
    GlobalSymbolTable* parent_symbtab = symbol_tables[curr_level.first][curr_level.second];
    LocalSymbolTable *symbtab = new LocalSymbolTable(new_level, parent_symbtab);
    ((LocalSymbolTable *)parent_symbtab)->children.push_back(symbtab);
    if(symbol_tables.size() <= new_main_level) symbol_tables.push_back({symbtab});
    else symbol_tables[new_main_level].push_back(symbtab);
    current_level = level_stack.top();
    return ;
}

pair<int,int> GlobalSymbolTable::get_next_level(){
    pair<int,int> curr_level = level_stack.top();
    int new_main_level = curr_level.first + 1;
    int new_sub_level = (symbol_tables.size() <= new_main_level) ? 0 : symbol_tables[new_main_level].size();
    pair<int,int> new_level = {new_main_level, new_sub_level};
    return new_level;
}

void GlobalSymbolTable::decrease_level(){
    level_stack.pop();
    if(level_stack.empty()){
        // throw error
    }
    current_level = level_stack.top();
    return ;
}

GlobalSymbolTable::GlobalSymbolTable(){
    current_level = {0,0};
    level_stack.push(current_level);
    symbol_tables.resize(1, vector<GlobalSymbolTable*> (1));
}

LocalSymbolTable::LocalSymbolTable(pair<int,int> level, GlobalSymbolTable* assign_parent){
    curr_level.first = level.first;
    curr_level.second = level.second;
    parent = assign_parent;
}

// Define an array of strings that corresponds to the modifier types.
const string modifierStrings[] = {"public", "protected", "private", "abstract", "static", "sealed", "nonsealed", "strictfp", "transitive", "final", "volatile", "synchronized", "transient", "native"};

// Modifier list - PUBLIC (0), STATIC (4), FINAL (9), PRIVATE (2)
void LocalSymbolTable::add_entry(Node* symtab_entry){
    hashed_names.insert({symtab_entry->name, symbol_table_entries.size()});
    symbol_table_entries.push_back(symtab_entry);
    // cout<<symtab_entry->lexeme<<" "<<symtab_entry->line_no<<" "<<symtab_entry->name<<endl;
    if(symtab_entry->entry_type == CLASS_DECLARATION){
        NormalClassDeclaration* temp = (NormalClassDeclaration*)(symtab_entry);
        // cout<<"class entry added: "<<(symtab_entry->name)<<" at level: "<<global_symtab->current_level.first<<" "<<global_symtab->current_level.second<<endl;
        vector<Modifier*> modifiers = temp->modifiers_list->lists;
        // Count the number of each modifier to give error if there are duplicates
        map<ModifierType, int> counts;
        for (Modifier *element : modifiers) {
            counts[element->modifier_type]++;
        }
        string error;
        for (Modifier *element : modifiers)
        {
            if (element->modifier_type != 0 && element->modifier_type != 9){
                error = "Modifier " + modifierStrings[element->modifier_type] + " not allowed in class declaration";
                yyerror(error.c_str());
                break;
            }
        }
        for (auto z : counts) {
            if (z.second > 1) {
                error = "Duplicate modifier found for the class - " + modifierStrings[z.first];
                yyerror(error.c_str());
                break;
            }
        }
        // throw error if bad modifier list combination done
        // throw error if constructor method name doesnt match with the class name
    }
    else if(symtab_entry->entry_type == METHOD_DECLARATION){
        MethodDeclaration* temp = (MethodDeclaration*)(symtab_entry);
        // cout<<"method entry added: "<<(symtab_entry->name)<<" at level: "<<global_symtab->current_level.first<<" "<<global_symtab->current_level.second<<endl;
        // cout<<temp->formal_parameter_list->lists[0]->variable_declarator_id->num_of_dims;
        vector<Modifier*> modifiers = temp->modifiers->lists;
        // Count the number of each modifier to give error if there are duplicates
        map<ModifierType, int> counts;
        for (Modifier *element : modifiers) {
            counts[element->modifier_type]++;
        }
        string error;
        int c = 0;
        for (Modifier *element : modifiers)
        {
            if (element->modifier_type == 0 || element->modifier_type == 2){
                c++;
            }
            if (element->modifier_type != 0 && element->modifier_type != 9 && element->modifier_type != 4 && element->modifier_type != 2){
                error = "Modifier " + modifierStrings[element->modifier_type] + " not allowed in method declaration";
                yyerror(error.c_str());
                break;
            }
        }
        for (auto z : counts) {
            if (z.second > 1) {
                error = "Duplicate modifier found for the method - " + modifierStrings[z.first];
                yyerror(error.c_str());
                break;
            }
        }
        if (c > 1){
            error = "Methods can only set one of public / private access modifiers.";
            yyerror(error.c_str());
        }
        if(temp->isConstructor){
            // throw error if bad modifier list combination done
        }
    }
    else if(symtab_entry->entry_type == VARIABLE_DECLARATION){
        LocalVariableDeclaration* temp = (LocalVariableDeclaration*)(symtab_entry);
        vector<Modifier *> modifiers = temp->modifiers_lists->lists;
        // Count the number of each modifier to give error if there are duplicates
        map<ModifierType, int> counts;
        for (Modifier *element : modifiers) {
            counts[element->modifier_type]++;
        }
        string error;
        for (auto z : counts) {
            if (z.second > 1) {
                error = "Duplicate modifier found for the field variable - " + modifierStrings[z.first];
                yyerror(error.c_str());
                break;
            }
        }
        if (temp->isFieldVariable == true)
        {
            int c = 0;
            for (Modifier *element : modifiers)
            {
                if (element->modifier_type == 0 || element->modifier_type == 2){
                    c++;
                }
                if (element->modifier_type != 0 && element->modifier_type != 9 && element->modifier_type != 4 && element->modifier_type != 2){
                    error = "Modifier " + modifierStrings[element->modifier_type] + " not allowed in field variable declaration";
                    yyerror(error.c_str());
                    break;
                }
            }
            if (c > 1){
                error = "Methods can only set one of public / private access modifiers.";
                yyerror(error.c_str());
            }
        }
        else {
            for (Modifier *element : modifiers)
            {
                cout << "2 ";
                if (element->modifier_type != 9){
                    cout << "1 ";
                    error = "Modifier " + modifierStrings[element->modifier_type] + " not allowed in field variable declaration";
                    yyerror(error.c_str());
                    break;
                }
            }
        }
        
        if(temp->type->primitivetypeIndex == -1){
            // cout<<"object of class type: "<<temp->type->class_instantiated_from->name<<" declared named : "<<temp->name<<" \n";
        }
        // cout<<"initialized value: "<<temp->variable_declarator->initialized_value->num_val[0];
        // cout<<temp->variable_declarator->num_of_dims<<endl;
        
        // throw error if bad modifier list combination done
    }
    else {

    }
    return ;
}

Node* LocalSymbolTable::get_entry(string name, int entry_type){
    // nested scope 
    LocalSymbolTable* temp = this;
    int pos = name.find('.');
    if(pos != string::npos) name = name.substr(0, pos);

    while(temp != NULL){
        if(temp->hashed_names.find(name)!=temp->hashed_names.end()){
            Node* res = temp->symbol_table_entries[temp->hashed_names[name]];
            // cout<<res->name<<endl;
            if((entry_type == -1) || (entry_type == (int)(res->entry_type))) return res;
            else temp = (LocalSymbolTable*)(temp->parent);
        }
        else{
            temp = (LocalSymbolTable*)(temp->parent);
        }
    }
    // for input of type x it returns x;
    // for input of type obj.x it returns obj;
    return NULL;
}

LocalSymbolTable* get_local_symtab(pair<int,int> curr_level){
    if(global_symtab->symbol_tables.size() <= curr_level.first || global_symtab->symbol_tables[curr_level.first].size() <= curr_level.second){
        // throw compiler error;
    }
    else return ((LocalSymbolTable*)(global_symtab->symbol_tables[curr_level.first][curr_level.second]));
}