/* An ASCII STL file begins with the line 
solid name

where name is an optional string (though if name is omitted there must still be a 
space after solid). The file continues with any number of triangles, each 
represented as follows:

facet normal ni nj nk
    outer loop
        vertex v1x v1y v1z
        vertex v2x v2y v2z
        vertex v3x v3y v3z
    endloop
endfacet
endsolid name
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define jmfdebug

int main(int argc,char **argv)
{int fd,ret,len,k,l,attributes,getid=0;
 char filename[256],filetmp[256];
 int32_t *i,count;
 float *f;
 float *farray=NULL;
 int16_t *s;
 unsigned char c[256];
 FILE *file;
 if (argc>1) fd=open(argv[1],O_RDONLY);
 else fd=open("s1_platform.g3db",O_RDONLY);
 read(fd,c,1);
 if (c[0]!='{') {printf("Error\n");exit(1);} else printf("%c\n",c[0]);
 do 
 {ret=read(fd,c,1);
  switch (c[0])
  { case 's': printf("string: ");
              read(fd,c,1);          // 1 byte length
              if (c[0]>0) {ret=read(fd,&c,c[0]);  // string
                           c[ret]=0;
                           printf("%s ",c);
                           if ((strcmp(c,"attributes")==0)) attributes=0;
                           if (getid==1) {sprintf(filename,"%s",c);getid=0;} // arg of id is filename
                           if ((strcmp(c,"id")==0)) getid=1;
                           if ((strcmp(c,"POSITION")==0)||(strcmp(c,"NORMAL")==0)||(strcmp(c,"TANGENT")==0)||(strcmp(c,"BINORMAL")==0))
                              attributes+=3;
                           if (strcmp(c,"COLOR")==0) attributes+=4;
                           if ((strcmp(c,"TEXCOORD")==0)||(strcmp(c,"TEXCOORD0")==0)||(strcmp(c,"BLENDWEIGHT")==0)) attributes+=2;
                           if ((strcmp(c,"meshes")==0) || (strcmp(c,"parts")==0) || (strcmp(c,"parts")==0))
                              {ret=read(fd,&c,2);  // string
                               printf("%c%c\n",c[0],c[1]);
                              }
                           else printf("\n");
                          }
              break;
    case 'a': printf("array: ");
              read(fd,c,1);          // array type, 'i'==int16
              if (c[0]=='i') 
                {read(fd,c,1);      
                 count=c[0];
                 printf(" short%d: ",count); 
                 if (attributes>0)
                   {sprintf(filetmp,"%s_ai.stl",filename);
                    file=fopen(filetmp,"w");
                    fprintf(file,"solid %s\n",filename);
                   }
                 for (k=0;k<count/3;k++) // TRIANGLE
                    {if (attributes>0) fprintf(file,"facet normal 1 1 1\nouter loop\n"); // CORRIGER
                     for (l=0;l<3;l++)
                      {read(fd,c,2);
                       s=(int16_t*)c; *s=htons(*s);
                       // fprintf(file,"%d ",*s);
                       if (attributes>0)
                          fprintf(file,"vertex %f %f %f\n",farray[*s*attributes],farray[*s*attributes+1],farray[*s*attributes+2]);
                       else printf("%x ",*s);
                      }
                     if (attributes>0) fprintf(file,"endloop\nendfacet\n");
                    }
                 if (attributes>0)
                   {fprintf(file,"endsolid %s\n",filename);
                    fclose(file);
                   }
                 else printf("\n");
                }
              else
                if (c[0]=='d') 
                  {read(fd,c,1);      
                   len=c[0];
                   printf(" float%d: ",len); 
                   for (k=0;k<len;k++) 
                    {read(fd,c,4);
                     i=(int32_t*)c;*i=ntohl(*i);
                     f=(float*)i;
                     printf("%f ",*f);
                    }
                   printf("\n");
                  }
              break;
    case 'A': printf("Array: ");
              read(fd,c,1); printf("type: %c=",c[0]);
              if (c[0]=='d') // array type, 'd'==float
                {printf(" float: "); 
                 read(fd,c,4);
                 i=(int32_t*)c;
                 count=htonl(*i); 
// https://ubjson.org/#endian
// The Universal Binary JSON specification requires that all numeric values be written 
// in Big-Endian order.
                 printf("count=0x%04x attributes=%d\n",count,attributes);
                 if (farray!=NULL) free(farray);
                 farray=(float*)malloc(count*sizeof(float));
                 for (k=0;k<count/(attributes);k++) // POSITION-NORMAL
                   {for (l=0;l<(attributes);l++)
                     {read(fd,c,4);
                      i=(int32_t*)c;
                      *i=htonl(*i);
                      f=(float*)i;
                      // fprintf(file,"%f ",*f);
                      farray[k*attributes+l]=*f;
                     }
                    // fprintf(file,"\n");
                   }
                }
              else
                if (c[0]=='i')  // array type, 'i'==short
                  {printf(" int16: ");
                   read(fd,c,4);
                   i=(int32_t*)c;
                   count=htonl(*i); 
                   printf("0x%04x\n",count);
                   sprintf(filetmp,"%s_Ai.stl",filename);
                   file=fopen(filetmp,"w");
                   fprintf(file,"solid %s\n",filename);
                   for (k=0;k<count/3;k++) // TRIANGLE
                     {fprintf(file,"facet normal 1 1 1\nouter loop\n"); // CORRIGER
                      for (l=0;l<3;l++)
                       {read(fd,c,2);
                        s=(int16_t*)c;
                        *s=htons(*s);
                        // fprintf(file,"%d ",*s);
                        fprintf(file,"vertex %f %f %f\n",farray[*s*attributes],farray[*s*attributes+1],farray[*s*attributes+2]); // CORRIGER
                     }
                    fprintf(file,"endloop\nendfacet\n");
                   }
                   fprintf(file,"endsolid %s\n",filename);
                   fclose(file);
                  }
              break;
    default:  if ((c[0]>=32)&&c[0]<0x80) printf("%c",c[0]); else printf("0x%02x\n",c[0]);
              break;
  }
 } while (ret>0);
 close(fd);
}

/*
00000000: 7b73 0776 6572 7369 6f6e 6169 0200 0000  {s.versionai....
00000010: 0173 0269 6473 0073 066d 6573 6865 735b  .s.ids.s.meshes[
00000020: 7b73 0a61 7474 7269 6275 7465 735b 7308  {s.attributes[s.
00000030: 504f 5349 5449 4f4e 7306 4e4f 524d 414c  POSITIONs.NORMAL
00000040: 5d73 0876 6572 7469 6365 7341 6400 01a3  ]s.verticesAd...
00000050: f4c0 fa6a ce41 05dc 0941 66b4 cabb 03e7  ...j.A...Af.....
00000060: 143e 328d 9d3f 7c13 e2c0 f77a 5241 05b2  .>2..?|....zRA..
00000070: fb41 66bc d4bb 03e7 153e 328d 9e3f 7c13  .Af......>2..?|.
00000080: e2c0 f7b4 da41 0b46 9d41 65bf f6bb 03e7  .....A.F.Ae.....
00000090: 143e 328d 9c3f 7c13 e2c0 fa6a 0441 05b9  .>2..?|....j.A..
*/
