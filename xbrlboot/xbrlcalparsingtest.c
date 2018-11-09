

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include <errno.h>

FILE* fdesc;

int ReadPacket(char* mem, int size)
{
    int res;
    res = fread(mem, 1, size, fdesc);
    return res;
}

void ParseCalXbrlFile(const char* file)
{
    // initialize
    LIBXML_TEST_VERSION;
    xmlDocPtr doc;
    
    fdesc = fopen(file, "rb");
    if (fdesc != NULL)
    {
        xmlParserCtxtPtr ctxt;
        char chars[4];
        int res;
        
        res = ReadPacket(chars, 4);
        if (res <= 0)
        {
            fprintf(stderr, "Failed to parse %s.\n", file);
            goto closefile;
        }
        
        ctxt = xmlCreatePushParserCtxt(NULL, NULL, chars, res, file);
        if (ctxt == NULL)
        {
            fprintf(stderr, "Failed to create parser context.\n.");
            goto closefile;
        }
        
        while ( (res = ReadPacket(chars, 4))> 0 )
        {
            xmlParseChunk(ctxt, chars, res, 0);
        }
        
        xmlParseChunk(ctxt, chars, 0, 1);
        
        doc = ctxt->myDoc;
        res = ctxt->wellFormed;
        
        xmlFreeParserCtxt(ctxt);
        
        if (!res)
        {
            fprintf(stderr, "Failed to parse %s.\n", file);
            goto freedoc;
        }
    }
    else
    {
        const char* msg = strerror(errno);
        fprintf(stderr, msg);
        goto exitfunc;
    }
    
freedoc:
        xmlFreeDoc(doc);

closefile:
        fclose(fdesc);
    
exitfunc:
    xmlCleanupParser();
    xmlMemoryDump();
}


