#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillInodeTable(uint32_t itotal, bool set_date)
    {
        soProbe(604, "%s(%u)\n", __FUNCTION__, itotal);
        
        /* change the following line by your code */
        // return binFillInodeTable(itotal, set_date);

       // Criar inode table

        SOInode inodetable[IPB];    // Tabela de inodes. IPB = Inode Per Block

        uint32_t total_blocks = itotal/IPB;     // Numero total de blocos: Número total de inodes / Inodes Per Block
        int index = 0;
            
        for(int i = 1; i <= total_blocks + 1 ;i++){  // Percorrer os blocos da tabela de inode   * Verificar <= 


            if(i == 1)              // inode 0 é guardado no bloco 0
            {   
                index++;
                SOInode inode;              // Criar inode
                inode.mode = S_IFDIR | 00775;
                inode.lnkcnt = 2;
                inode.owner = getuid();
                inode.group = getgid();
                inode.size = 1024;
                inode.blkcnt = 1;
                if(set_date == true)
                {
                    inode.atime = time(NULL);
                    inode.mtime = time(NULL);
                    inode.ctime = time(NULL);
                }
                else
                {
                    inode.atime = 0;
                    inode.mtime = 0;
                    inode.ctime = 0;
                }
                
                for(int j = 0; j < N_DIRECT; j++)
                {
                    if(j == 0)
                    {
                        inode.d[j] = 0;
                    }
                    else
                    {
                        inode.d[j] = NullReference;   
                    }
                }
                for(int j = 0; j < N_INDIRECT; j++)
                {
                    inode.i1[j] = NullReference;
                }
                for(int j = 0; j < N_DOUBLE_INDIRECT; j++)
                {
                    inode.i2[j] = NullReference;
                }

                inodetable[0] = inode;

                for(uint32_t k = 1; k < IPB; k++)
                {
                    index++;
                    SOInode inode;
                    inode.mode = INODE_FREE;
                    inode.lnkcnt = 0;
                    inode.owner = 0;
                    inode.group = 0;
                    inode.size = 0;
                    inode.blkcnt = 0;
                    inode.atime = 0;
                    inode.mtime = 0;
                    inode.ctime = 0;
                    inode.next = index;
                    
                    for(int j = 0; j < N_DIRECT; j++)
                    {
                        inode.d[j] = NullReference;
                    }
                    for(int j = 0; j < N_INDIRECT; j++)
                    {
                        inode.i1[j] = NullReference;
                    }
                    for(int j = 0; j < N_DOUBLE_INDIRECT; j++)
                    {
                        inode.i2[j] = NullReference;
                    }

                    inodetable[k] = inode;
                }
            }
            else                // inode table começa no bloco 1
            {
                for(uint32_t k = 0; k < IPB; k++)
                {
                    index++;
                    SOInode inode;

                    inode.mode = INODE_FREE;
                    inode.lnkcnt = 0;
                    inode.owner = 0;
                    inode.group = 0;
                    inode.size = 0;
                    inode.blkcnt = 0;
                    inode.atime = 0;
                    inode.mtime = 0;
                    inode.ctime = 0;
                    if(k == IPB-1 && i == total_blocks)
                    {
                        inode.next = NullReference;
                    }
                    else
                    {
                        inode.next = index;
                    }
                    for(int j = 0; j < N_DIRECT; j++)
                    {
                        inode.d[j] = NullReference;
                    }
                    for(int j = 0; j < N_INDIRECT; j++)
                    {
                        inode.i1[j] = NullReference;
                    }
                    for(int j = 0; j < N_DOUBLE_INDIRECT; j++)
                    {
                        inode.i2[j] = NullReference;
                    }

                    inodetable[k] = inode;
                }
            }
    
        soWriteRawBlock(i,inodetable);
            
        }

    }

};

