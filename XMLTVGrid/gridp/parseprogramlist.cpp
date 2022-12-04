#include "parseprogramlist.h"

#include <iostream>
#include <assert.h>

namespace programp {

static xmlNode *cur_pagenode = NULL;
static xmlDoc *doc = NULL;
static xmlNode *root_element = NULL;

std::string get(const xmlChar *element)
{
    xmlNode *cur_node;
    //assert(cur_node);

    for (cur_node = cur_pagenode->children; cur_node; cur_node = cur_node->next) {
        assert(cur_node);
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, element)){
                return std::string((char *)xmlNodeGetContent(cur_node));
            }
        }
    }
    assert(false);
}

bool nextProgram()
{
    assert(cur_pagenode);
    cur_pagenode = cur_pagenode->next;

    while(cur_pagenode) {
        if(cur_pagenode->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_pagenode->name, programp::program)){
                return cur_pagenode;
            }
        }
        cur_pagenode = cur_pagenode->next;
    }

    return NULL;
}

void getProgramsNode(xmlNode * a_node)
{
    if (a_node->type == XML_ELEMENT_NODE) {
        if (!xmlStrcmp(a_node->name, programp::programs)){
            cur_pagenode = a_node->children;
        }else{
            std::cout << a_node->name <<std::endl;
            assert(false);
        }
    }
}

bool initPrograms(const char* xmlfile_in)
{
    cur_pagenode = NULL;
    doc = NULL;
    root_element = NULL;

    /*parse the file and get the DOM */
    doc = xmlReadFile(xmlfile_in, NULL, 0);

    if (doc == NULL) {
        fprintf(stderr, "error: could not parse file %s\n", xmlfile_in);
        return false;
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    getProgramsNode(root_element);

}

void closePrograms()
{
    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}
}
