import { readFile } from 'node:fs/promises';
import { WASI } from 'wasi';
import { argv, env } from 'node:process';
import { dirname, join } from 'node:path';

const wasi = new WASI({
  version: 'preview1',
  args: argv,
  env,
});
const module = await WebAssembly.compile(await readFile(join(dirname(argv[1]), 'xitca-web.wasm')));
const instance = await WebAssembly.instantiate(module, wasi.getImportObject());

wasi.start(instance);
