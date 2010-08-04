/* not finished yet */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static int 
goodfile(char *name, int exec)
{
  FILE *inf;
  short magic;
  
  inf = fopen(name, "r");
  if (!inf)
    return 0;
  if (exec) 
    {
      magic = 0;
      fread(&magic, 2, 1, inf);
      fclose(inf);
      if ((magic == 0x0160) || (magic == 0x7f45))
	return 1;
      else
	fclose(inf);
      return 1;
    }
}

static void
locatefile(char *name, char *expname, char *domain, int exec)
{
  char *path;
  char *start, *finish;
  char save;
  char template[256];
  
  if(name[0] == '/' || name[0] == '.') 
    {
      strcpy(expname, name);
      return;
    }
  if(goodfile(name, exec)) 
    {
      strcpy(expname, name);
      return;
    }
  path = (char*)getenv(domain);
  if (!path)
    {
      strcpy(expname, name);
      return;
    }
  start = path;
  finish = path;
  while(1)
    {
      if(*path == 0 || *path == ':') 
	{
	  if (start != finish) 
	    {
	      save = *finish;
	      *finish = 0;
	      strcpy(template, start);
	      *finish = save;
	      strcat(template,"/");
	      strcat(template, name);
	      if (goodfile(template,exec)) 
		{
		  strcpy(expname,template);
		  return;
		}
	    }
	  if (*path == 0)
	    {
	      strcpy(expname, name);
	      return;
	    }
	  path++;
	  start = path;
	  finish = path;
	}
      else
	{
	  path++;
	  finish = path;
	}
    }
	  
}

void findname(char *name, char *expname, char *domain)
{
  locatefile(name, expname, domain, 0);
}



