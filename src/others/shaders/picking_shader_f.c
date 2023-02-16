#version 450 core

out int ObjectID;

uniform int u_ObjectID;

void main() {  //
  ObjectID = u_ObjectID;
}