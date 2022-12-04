#include "parsewikipedia.h"
#include <iostream>
#include <assert.h>

namespace wikip {

static xmlNode *cur_pagenode = nullptr;
static xmlDoc *doc = nullptr;
static xmlNode *root_element = nullptr;

std::string currentTitle()
{
    xmlNode *cur_node = nullptr;
    assert(cur_node);

    for (cur_node = cur_pagenode->children; cur_node; cur_node = cur_node->next) {
        assert(cur_node);
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, wikip::title)){
                return std::string((char *)xmlNodeGetContent(cur_node));
            }
        }
    }
    assert(false);
}

bool nextPage()
{
    assert(cur_pagenode);
    cur_pagenode = cur_pagenode->next;

    while(cur_pagenode) {
        if(cur_pagenode->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_pagenode->name, wikip::page)){
                return cur_pagenode;
            }
        }
        cur_pagenode = cur_pagenode->next;
    }

    return NULL;
}

void getMediawikiNode(xmlNode * a_node)
{
    if (a_node->type == XML_ELEMENT_NODE) {
        if (!xmlStrcmp(a_node->name, wikip::mediawiki)){
            cur_pagenode = a_node->children;
        }else{
            std::cout << a_node->name <<std::endl;
            assert(false);
        }
    }
}

void initMediawiki(const char* xmlfile_in)
{
    cur_pagenode = NULL;
    doc = NULL;
    root_element = NULL;

    /*parse the file and get the DOM */
    doc = xmlReadFile(xmlfile_in, NULL, 0);

    if (doc == NULL) {
        fprintf(stderr, "error: could not parse file %s\n", xmlfile_in);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    getMediawikiNode(root_element);

}

void closeMediawiki()
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
