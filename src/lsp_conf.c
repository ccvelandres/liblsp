#include "lsp.h"
#include "lsp_log.h"

static const char *tag = "lsp_conf";

static lsp_conf_t _lsp_conf = {
    .addr = LSP_DEFAULT_ADDR,
    .hostname = LSP_DEFAULT_HOSTNAME,
    .machinename = LSP_DEFAULT_MACHINENAME,
    .rev = LSP_DEFAULT_LSPREV,
    .conn_max = LSP_DEFAULT_MAX_CONNECTIONS,
    .conn_queuelen = LSP_DEFAULT_CONN_QUEUELEN};

const lsp_conf_t *const lsp_conf = &_lsp_conf;

int lsp_conf_init(lsp_conf_t *conf)
{
    if (conf == NULL)
    {
        lsp_verb(tag, "Default configurations loaded\n");
        return LSP_ERR_NONE;
    }

    if(conf->addr > LSP_PACKET_ADDR_MAX) {
        lsp_verb(tag, "conf: address out of range\n");
        return -LSP_ERR_ADDR_INVALID;
    }

    if(conf->hostname == NULL)
        lsp_verb(tag, "conf: null hostname, loading defaults\n");

    if(conf->machinename == NULL)
        lsp_verb(tag, "conf: null machinename, loading defaults\n");

    if(conf->rev == NULL)
        lsp_verb(tag, "conf: null rev, loading defaults\n");

    _lsp_conf.addr = conf->addr;
    _lsp_conf.hostname = conf->hostname;
    _lsp_conf.machinename = conf->machinename;
    _lsp_conf.rev = conf->rev;
    _lsp_conf.conn_max = conf->conn_max;
    _lsp_conf.conn_queuelen = conf->conn_queuelen;

    return LSP_ERR_NONE;
}