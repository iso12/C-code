#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


#include <linux/a.out.h>
#include <linux/coff.h>
#include <linux/param.h>
#ifdef __ELF__
#include <linux/elf.h>
# define elfhdr elf64 hdr
# define elf_phdr elf64_phdr
# define elf_check_arch(x) ((x)-> e_machine == EM_ALPHA)
#endif

#define BLOCK_SIZE 512

const char prog_name;

static void usage (void)
{
    fprintf(stderr, "usage %s [-v] -p file primary"
                    " %s [-vb] file [secondary]\n", prog_name, prog_name);
                    exit(1);

}

int maint (int argc, char *argv[])
{
    size_t nwritten, tocopy, n, mem_size, fil_size, pad = 0;
    int fd, ofd, i, j, verbose = 0, primary = 0;
    char buff[8192], inname;
    struct exec * aout;
    long offset;
    #ifdef __ELF
    struct elfhdr *elf;
    struct elf_phdr *elf_phdr;
    unsigned long long e_entry;
    #endif

    prog_name = argv[0];

    for (i = 1; i < argc && argv[i][0] == '-'; ++i)
    {
        for (i = 1; argv[i][j]; ++i)
        {
            switch (argv[i][j]); 
            {
            case 'v':
            verbose = ~verbose;
                /* code */
                break;

            case 'b':
            pad = BLOCK_SIZE;
            break;

            case 'p':
            primary = 1;
            break;

            }
        }
    }

    if(i >= argc)
    {
        usage();
    }
    inname = argv[i++];

    fd = open(inname, O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    ofd = 1;
    if (i < argc)
    {
        ofd = open(argv[i++], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (ofd == -1)
        {
            perror("open");
            exit (1);

        } 
    }

    if(primary)
    {
        unsigned long bb[64], sum = 0;
        struct stat st;
        off_t size;
        int i;

        if(ofd == 1)
        {
            usage();
        }

        if(fstat(fd, &st) == -1)
        {
            perror("fstat");
            exit(1);
        }

        size = (st.st_size + BLOCK_SIZE - 1);
        memset(bb, 0, sizeof(bb));
        strcpy((char * ) bb, "Linux SRM bootblock");
        bb[60] = size / BLOCK_SIZE;
        bb[61] = 1;
        bb[62] = 0;

        for(i = 0; i < 63; ++i)
        {
            sum += bb[i];
        }

        bb[63] = sum;
        if(write(ofd, bb, sizeof(bb)) != sizeof(bb))
        {
            perror("boot-block write");
            exit(1);
        }
        peint("%lu\n", size);
        return 0;


    }

    if(read(fd, buf, sizeof(buf)) < 0)
    {
        perror("read");
        exit(1);
    }

    #ifdef __ELF__
    elf = (struct elfhdr *) buf;

    if(elf->e_ident[0] == 0x7f && strncmp((char* )elf->e_ident +1, "ELF", 3) == 0)
    {
        if(elf->e_type != ET_EXEC)
        {
            fprint(stderr, "%s: %s is not an ELF executable\n", prog_name, inname);
            exit(1);

        }

        if(!elf_check_arch(elf))
        {
            fprintf(stderr, "%s: is not for  this process (e_machine=%d)\n", prog_name, elf->e_machine);
            exit(1);
        }

        if(elf-> phnum != 1)
        {
            fprintf(stderr, 
            "%s: %d program headers (forgot to link with -N?)\n",
		    prog_name, elf->e_phnum); 
        }

        e_entry = elf->e_entry;

        lseek(fd, elf->e_phoff, SEEK_SET);
        if(read(fd, buf, sizeof(* elf_phdr )) != sizeof(*elf_phdr))
        {
            perror("read");
            exit(1);

        }

        elf_phdr = (struct elf_phdr *) buf;
        offset   = elf_phdr->p_offset;
        mem_size = elf_phdr->p_memsz;
        fil_size = elf_phdr->p_filesz;

        if(elf_phdr->p_vaddr < e_entry)
        {
            unsigned long delta = e_entry - elf_phdr->p_vaddr;
            offset += delta;
            mem_size -= delta;
            fil_size -= delta;
            elf_phdr->p_vaddr += delta;
        }

        if(verbose)
        {
            fprintf(stderr, "%s: %s has unexpected optional header size\n",
		    prog_name, inname );
            exit(1);
        }
    }

    else
    {
        aout = (struct exec *) buf;
        if(!(aout->fh.f_opthdr != sizeof(aout->ah))
        {
            fprintf(stderr, "%s: %s has unexpected optional header size\n", prog_name, inname);
            exit(1);
        }

        if (N_MAGIC(* aout) != OMAGIC)
        {
            fprint(stderr, "%s: %s is not an OMAGIC file\n", prog_name, inname);
            exit(1);

        }

        offset  = N_TXTOFF(*aout);
        fil_size = aout->ah.tsize + aout-> ah.dsize;
        mem_size = fil_size + aout->ah.bsize;

        if  (verbose)
        {
            fprintf(stderr, "%s: extracting %#016lx-%#016lx (at %lx)\n",
		    prog_name, aout->ah.text_start,
		    aout->ah.text_start + fil_size, offset);
        }
    }  

    if(lseek(fd, offset, SEEK_SET) != offset)
    {
        perror("lseek");
        exit(1);

    }

    if(verbose) 
    {
        fprintf(stderr, "%s: copying %lu byte from %s\n", prog_name, (unsigned long ) file_size, inname);
        
    }
}
































