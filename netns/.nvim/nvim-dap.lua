local dap = require('dap')

dap.adapters.lldb = {
    type = 'executable',
    command = "/usr/bin/lldb-vscode",
    name = "lldb"
}

 dap.configurations.c = {
 {
    name = "mknetns-mount-path",
    type = "lldb",
    request = 'launch',
    program = "mknetns",
    cwd = '${workspaceFolder}',
    stopOnEntry = false,
    args = {"-m", "/var/run/net-test"},
 },
 {
    name = "mknetns-join-path",
    type = "lldb",
    request = 'launch',
    program = "mknetns",
    cwd = '${workspaceFolder}',
    stopOnEntry = false,
    args = {"-j", "test"},
 },
}
