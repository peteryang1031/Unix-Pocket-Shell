# Unix-Pocket-Shell
This shell is not compatible with Windows Command and only applied for Unix:D.
## Man Page
### ls

**SYNOPSIS**
  ls [options]...[FILE]...
  
**DESCRIPTION**

  List information about the FILES(the current directory by default).
  
  Arguments can be:
  * **-a**
      do not ignore entries starting with .
  * **-A**
      do not list implied . and ..
  * **-l**
      use a long listing format,including from file mode, number of nlinks,user name, group name, size, mode modification time      and name left to right
  * **-R**
      list subdirectories recursively
      
### cat 

**SYNOPSIS**
  cat [options]...[FILE]...
  
**DESCRIPTION**

  * **cat**
        Concatenate FILE(s) to standard output
  * **cat file**
        display all contents of file
  * **cat >file<<EOF**
        Create file and write to it from standard input
  * **cat >>file<<EOF""
        Append content to an existing file from standard input
### cd

**SYNOPSIS**
  cd [FILE]...

**DESCRIPTION**
  
  * **cd** 
        Modify current directory to user directory
  * **cd PATH**
        Modify current directory to PATH
       
