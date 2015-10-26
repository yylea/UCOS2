#ifndef __DOS_H
#define __DOS_H

#include <_defs.h>
#include <stddef.h>

#ifdef __WATCOMC__
#pragma pack( __push, 1 )
#endif

struct _R16BIT {
    unsigned short ax, bx, cx, dx, si, di, es, cs, ss, ds, flags;
    unsigned char cflag;
};

struct _R8BIT {
    unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union _INTR {
    struct _R16BIT x;
    struct _R8BIT h;
};
#define INTR _INTR

struct country {
    int co_date;         /* date format */
    char co_curr[ 5 ];   /* currency symbol */
    char co_thsep[ 2 ];  /* thousands separator */
    char co_desep[ 2 ];  /* decimal separator */
    char co_dtsep[ 2 ];  /* date separator */
    char co_tmsep[ 2 ];  /* time separator */
    char co_currstyle;   /* currency style */
    char co_digits;      /* significant digits in currency */
    char co_time;        /* time format */
    long co_case;        /* case map */
    char co_dasep[ 2 ];  /* data separator */
    char co_fill[ 10 ];  /* filler */
};
#define COUNTRY country

struct DOSERROR {
    int  de_exterror;     /* extended error */
    char de_class;        /* error class */
    char de_action;       /* action */
    char de_locus;        /* error locus */
};

struct date {
    unsigned int da_year;     /* current year */
    unsigned char da_day;     /* day of the month */
    unsigned char da_mon;     /* month (1 = Jan) */
};

struct dosdate_t {
    unsigned char day;        /* 1--31 */
    unsigned char month;      /* 1--12 */
    unsigned int  year;       /* 1980--2099 */
    unsigned char dayofweek;  /* 0--6; 0 = Sunday */
};

struct devhdr {
    long            dh_next;
    short           dh_attr;
    unsigned short  dh_strat;
    unsigned short  dh_inter;
    char            dh_name[ 8 ];
};

struct dfree {
    unsigned df_avail;  /* Available clusters */
    unsigned df_total;  /* Total clusters */
    unsigned df_bsec;   /* Bytes per sector */
    unsigned df_sclus;  /* Sectors per cluster */
};

struct diskfree_t {
    unsigned total_clusters;
    unsigned avail_clusters;
    unsigned sectors_per_cluster;
    unsigned bytes_per_sector;
};

typedef struct {
    char   drive;
    char   pattern [ 13 ];
    char   reserved [ 7 ];
    char   attrib;
    short  time;
    short  date;
    long   size;
    char   nameZ [ 13 ];
} dosSearchInfo;

struct fatinfo {
    char  fi_sclus;  /* sectors per cluster */
    char  fi_fatid;  /* the FAT id byte */
    int   fi_nclus;  /* number of clusters */
    int   fi_bysec;  /* bytes per sector */
};

struct ffblk {
#ifdef __CLIB_LFN__
    unsigned short cr_time;  /* time of file creation      */
    unsigned short cr_date;  /* date of file creation      */
    unsigned short ac_time;  /* time of last file access   */
    unsigned short ac_date;  /* date of last file access   */
    char ff_reserved[ 13 ];  /* reserved for use by DOS    */
#else
    char ff_reserved[ 21 ];  /* reserved for use by DOS    */
#endif
    char ff_attrib;          /* attribute byte for file    */
    unsigned short ff_ftime; /* time of last write to file */
    unsigned short ff_fdate; /* date of last write to file */
    unsigned long  ff_fsize; /* length of file in bytes    */
#ifdef __CLIB_LFN__
    char ff_name[ 256 ];     /* null-terminated filename   */
    unsigned short lfnhandle;/* DOS LFN support handle     */
#else
    char ff_name[ 13 ];      /* null-terminated filename   */
#endif
};

struct find_t {
#ifdef __CLIB_LFN__
    unsigned short cr_time;  /* time of file creation      */
    unsigned short cr_date;  /* date of file creation      */
    unsigned short ac_time;  /* time of last file access   */
    unsigned short ac_date;  /* date of last file access   */
    char reserved[ 13 ];     /* reserved for use by DOS    */
#else
    char reserved[ 21 ];     /* reserved for use by DOS    */
#endif
    char attrib;             /* attribute byte for file    */
    unsigned short wr_time;  /* time of last write to file */
    unsigned short wr_date;  /* date of last write to file */
    unsigned long  size;     /* length of file in bytes    */
#ifdef __CLIB_LFN__
    char name[ 256 ];        /* null-terminated filename   */
    unsigned short lfnhandle;/* DOS LFN support handle     */
#else
    char name[ 13 ];         /* null-terminated filename   */
#endif
};
#define _find_t find_t

struct fcb {
    char   fcb_drive;
    char   fcb_name[ 8 ],
           fcb_ext[ 3 ];
    short  fcb_curblk,
           fcb_recsize;
    long   fcb_filsize;
    short  fcb_date;
    char   fcb_resv[ 10 ],
           fcb_currec;
    long   fcb_random;
};

struct xfcb {
    char         xfcb_flag;
    char         xfcb_resv[ 5 ];
    char         xfcb_attr;
    struct  fcb  xfcb_fcb;
};

struct dostime_t {
    unsigned char hour;         /* Hours */
    unsigned char minute;       /* Minutes */
    unsigned char second;       /* Seconds */
    unsigned char hsecond;      /* Hundredths of seconds */
};

struct time {
    unsigned char  ti_min;   /* minutes */
    unsigned char  ti_hour;  /* hours */
    unsigned char  ti_hund;  /* hundredths of seconds */
    unsigned char  ti_sec;   /* seconds */
};

#ifdef __WATCOMC__
#pragma pack( __pop )
#endif

extern  int          __getversion( void );
extern char **       environ;
#define              _version (*__getversion)()
extern unsigned char _osmajor;
extern unsigned char _osminor;

extern int           absread( int drive, int sects, long lsect, void *buffer );
extern int           abswrite( int drive, int sects, long lsect, void *buffer );
extern int           allocmem( unsigned size, unsigned *seg );
extern int           bdos( int ah, unsigned dx, unsigned al );
extern int           bdosptr( int ah, void *argument, unsigned al );
extern int           _callint( union INTR *regs );
extern struct COUNTRY *
                     country( int xcode, struct COUNTRY *ct );
extern void          ctrlbrk( int ( *handler )( void ) );
extern void          delay( unsigned mill );
extern void          _disable( void );
extern unsigned      _dos_allocmem( unsigned size, unsigned *seg );
extern unsigned      _dos_close( int handle );
extern unsigned      _dos_creat( const char *path, int attr, unsigned *handle );
extern unsigned      _dos_creatnew( const char *path, int attr, unsigned *handle );
extern int __cdecl   dosexterror( struct DOSERROR *errblk );
extern unsigned      _dos_findfirst( char *filename, int attrib, void *strptr );
extern unsigned      _dos_findnext( void *strptr );
extern unsigned      _dos_findclose( void *strptr );
extern unsigned      _dos_freemem( unsigned seg );
extern void __cdecl  _dos_getdate( struct dosdate_t *ptr );
extern unsigned __cdecl
                     _dos_getdiskfree( unsigned char dr, struct diskfree_t *d );
extern unsigned      _dos_getdrive( unsigned *disk );
extern unsigned      _dos_getfileattr( const char *filename, unsigned *attrs );
extern unsigned __cdecl
                     _dos_getftime( int handle,
                                    unsigned *date,
                                    unsigned *time );
extern void __cdecl  _dos_gettime( struct dostime_t *timeptr );
extern void ( interrupt far *
                     _dos_getvect( unsigned intno ) )( );
extern void          _dos_keep( unsigned char retcode, unsigned size );
extern unsigned      _dos_open( const char *path, unsigned flags, unsigned *handle );
extern unsigned      _dos_read( int handle,
                                void far *buf,
                                unsigned len,
                                unsigned *nread );
extern unsigned      _dos_setblock( unsigned newsize,
                                    unsigned seg,
                                    unsigned *max );
extern void          _dos_setdate( struct dosdate_t *ptr );
extern void          _dos_setdrive( unsigned disk, unsigned *total );
extern unsigned      _dos_setfileattr( const char *filename, unsigned attrs );
extern unsigned      _dos_setftime( int handle, unsigned date, unsigned time );
extern unsigned      _dos_settime( struct dostime_t *timeptr );
extern void          _dos_setvect( unsigned intno,
                                   void ( interrupt far *vect )() );
extern time_t        dostounix( struct date *date, struct time *time );
extern unsigned      _dos_write( int handle,
                                 void far *buf,
                                 unsigned len,
                                 unsigned *bytes );
extern void          _enable( void );
extern int           freemem( unsigned seg );
extern int           getcbrk( void );
extern void __cdecl  getdate( struct date *datep );
extern void __cdecl  getdfree( unsigned char drive, struct dfree *dtable );
extern char        * getdta( void );
extern void          getfat( unsigned char drive, struct fatinfo *dtable );
extern void          getfatd( struct fatinfo *dtable );
extern unsigned      getpsp( void );
extern void __cdecl  gettime( struct time *timeptr );
extern void ( interrupt far *
                     getvect( int intno ) )( );
extern int           getverify( void );
extern int           inp( unsigned id );
extern unsigned      inpw( unsigned id );
extern unsigned      inport( unsigned id );
extern unsigned char inportb( unsigned id );
extern void          keep( unsigned char retcode, unsigned size );
extern void          nosound( void );
extern int           outp( unsigned id, int value );
extern unsigned      outpw( unsigned id, unsigned value );
extern void          outport( unsigned id, unsigned value );
extern void          outportb( unsigned id, unsigned char value );
extern char *        parsfnm( const char *cmdline, struct fcb *ptr, int al );
extern int           peek( unsigned seg, unsigned offs );
extern char          peekb( unsigned seg, unsigned offs );
extern void          poke( unsigned seg, unsigned offs, int value );
extern void          pokeb( unsigned seg, unsigned offs, char value );
extern int           randbrd( struct fcb *buf, int rnum );
extern int           randbwr( struct fcb *buf, int rnum );
extern int           setblock( unsigned newsize, unsigned seg );
extern int           setcbrk( int value );
extern void          setdate( struct date *datep );
extern void          setdta( char far *dta );
extern void          setpsp( unsigned psp );
extern void          settime( struct time *timeptr );
extern void          setvect( int intno, void ( interrupt far *vect )() );
extern void          setverify( int flag );
extern void          sleep( unsigned x );
extern void          sound( unsigned frequency );
extern void          unixtodos( time_t longtime,
                                struct date *date,
                                struct time *time );
extern int           unlink( const char *filename );

#define disable           _disable
#define enable            _enable

#define _A_NORMAL         0x00
#define _A_RDONLY         0x01
#define _A_HIDDEN         0x02
#define _A_SYSTEM         0x04
#define _A_VOLID          0x08
#define _A_SUBDIR         0x10
#define _A_ARCH           0x20
#define FA_NORMAL         _A_NORMAL
#define FA_RDONLY         _A_RDONLY
#define FA_HIDDEN         _A_HIDDEN
#define FA_SYSTEM         _A_SYSTEM
#define FA_LABEL          _A_VOLID
#define FA_DIREC          _A_SUBDIR
#define FA_ARCH           _A_ARCH

#define NFDS              20

#define SEEK_SET          0
#define SEEK_CUR          1
#define SEEK_END          2

#define MK_FP( seg, ofs ) ( ( void * )\
                          ( ( ( unsigned long )( seg ) << 16 ) |\
                          ( unsigned )( ofs ) ) )
#define FP_SEG(fp)        ( ( unsigned )( ( unsigned long )( fp ) >> 16 ) )
#define FP_OFF(fp)        ( ( unsigned )( fp ) )
#define __peek( a,b )     ( *( ( int far * )  MK_FP (  ( a ), ( b ) ) ) )
#define __peekb( a,b )    ( *( ( char far * ) MK_FP ( ( a ), ( b ) ) ) )
#define __poke( a,b,c )   ( *( ( int far * )  MK_FP ( (a),(b)) ) =  ( int )(c))
#define __pokeb( a,b,c )  ( *( ( char far * ) MK_FP ( (a),(b)) ) = ( char )(c))
#define peek( a,b )       ( *( ( int far * )  MK_FP ( ( a ), ( b ) ) ) )
#define peekb( a,b )      ( *( ( char far * ) MK_FP ( ( a ), ( b ) ) ) )
#define poke( a,b,c )     ( *( ( int far * )  MK_FP ( (a),(b)) ) =  ( int )(c))
#define pokeb( a,b,c )    ( *( ( char far * ) MK_FP ( (a),(b)) ) = ( char )(c))

#ifndef __NO_INLINE_FUNCTIONS
#pragma aux _enable  = "sti";
#pragma aux _disable = "cli";
#pragma aux inp      = "in al, dx"  parm [dx]      value [al] modify [ax dx];
#pragma aux inpw     = "in ax, dx"  parm [dx]      value [ax] modify [ax dx];
#pragma aux inport   = "in ax, dx"  parm [dx]      value [ax] modify [ax dx];
#pragma aux inportb  = "in al, dx"  parm [dx]      value [al] modify [ax dx];
#pragma aux outp     = "out dx, al" parm [dx] [al] value [al] modify [ax dx];
#pragma aux outpw    = "out dx, ax" parm [dx] [ax] value [ax] modify [ax dx];
#pragma aux outport  = "out dx, ax" parm [dx] [ax]            modify [ax dx];
#pragma aux outportb = "out dx, al" parm [dx] [al]            modify [ax dx];
#endif

#endif

