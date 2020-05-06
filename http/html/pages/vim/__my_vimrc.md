### My .vimrc (Ubuntu)

```
set nocompatible
filetype off

set tabstop=4
set shiftwidth=4
set expandtab
set autoindent
set termguicolors
set bg=dark
syntax on

set backspace=indent,eol,start
set ruler
" set number
set showcmd
set incsearch
set hlsearch

" Splitting
set splitbelow
set splitright

" My mappings
nmap <silent> \ <C-w>
" Experimental
nmap <silent> \\l :YcmCompleter GoToDefinition<CR>
nmap <silent> \\k :YcmCompleter GoToDeclaration<CR>
nmap <silent> \\j :YcmCompleter GoTo<CR>
nmap <silent> \\p :YcmCompleter GoToReferences<CR>
nmap <silent> \\t :YcmCompleter GetType<CR>
nmap <silent> \\i :YcmCompleter GoToInclude<CR>
" Action ones
nmap <silent> \\h :YcmCompleter FixIt<CR>
nmap <silent> \\; :TagbarOpenAutoClose<CR>
nmap <silent> \\n :NERDTreeToggle<CR>

" Clear highlighting on escape in normal mode
nnoremap <silent> <esc> :noh<return><esc>
nnoremap <silent> <esc>^[ <esc>^[

" Disable bell
set visualbell t_vb=

" CMake based builds
let &makeprg='make -C build -j4'

" Vundle part
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

Plugin 'VundleVim/Vundle.vim'

" Convenience
Plugin 'majutsushi/tagbar'
Plugin 'scrooloose/nerdtree'
Plugin 'terryma/vim-multiple-cursors'
"Plugin 'jiangmiao/auto-pairs'
Plugin 'cohama/lexima.vim'

" Themes
Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'
Plugin 'powerline/powerline'

" Colors
Plugin 'NLKNguyen/papercolor-theme'
Plugin 'nanotech/jellybeans.vim'
Plugin 'morhetz/gruvbox'
Plugin 'JBakamovic/yaflandia'
Plugin 'tomasiser/vim-code-dark'

" Code
Plugin 'Valloric/YouCompleteMe'
Plugin 'octol/vim-cpp-enhanced-highlight'
Plugin 'rhysd/vim-clang-format'

" Git
Plugin 'kablamo/vim-git-log'
Plugin 'gregsexton/gitv'
Plugin 'tpope/vim-fugitive'

call vundle#end()
filetype plugin indent on

" use patched fonts
let g:airline_powerline_fonts = 1

" show airline for tabs too
let g:airline#extensions#tabline#enabled = 1

" C++ code hightlight
"Highlighting of class scope is disabled by default. To enable set
let g:cpp_class_scope_highlight = 1

"Highlighting of member variables is disabled by default. To enable set
let g:cpp_member_variable_highlight = 1

"Highlighting of class names in declarations is disabled by default. To enable set
let g:cpp_class_decl_highlight = 1

"Highlighting of POSIX functions is disabled by default. To enable set
let g:cpp_posix_standard = 1

"Templates
let g:cpp_experimental_template_highlight = 1

"Highlighting of library concepts is enabled by
let g:cpp_concepts_highlight = 1

"
" YouCompleteMe options
"
let g:ycm_global_ycm_extra_conf = '~/.vim/.ycm_extra_conf.py'
" Supress confirmation question
let g:ycm_confirm_extra_conf = 0
" Errors locations list (:lp :ln)
let g:ycm_always_populate_location_list = 1
" Signature help  (experimental)
let g:ycm_auto_trigger = 1

" Syntax check
let g:ycm_register_as_syntastic_checker = 1 "default 1
let g:Show_diagnostics_ui = 1 "default 1
" will put icons in Vim's gutter on lines that have a diagnostic set.
" Turning this off will also turn off the YcmErrorLine and YcmWarningLine
" highlighting
let g:ycm_enable_diagnostic_signs = 1
let g:ycm_enable_diagnostic_highlighting = 1

" clangd recommended
" Let clangd fully control code completion
let g:ycm_clangd_uses_ycmd_caching = 0
" Use installed clangd, not YCM-bundled clangd which doesn't get updates.
let g:ycm_clangd_binary_path = exepath("clangd-10")
let g:ycm_clangd_args = [ 
    \ '--pch-storage=memory', 
    \ '--clang-tidy', 
    \ '--completion-style=bundled', 
    \ '--header-insertion=iwyu', 
    \ '-j=7', 
    \ '--pretty']

" clang-format
"let g:clang_format#command = "/usr/bin/clang-format-9"
let g:clang_format#detect_style_file = 1
"let g:clang_format#auto_format = 1
"let g:clang_format#auto_format_on_insert_leave = 1

" PaperColor color scheme settitgs
let g:PaperColor_Theme_Options = {
  \   'language': {
  \     'python': {
  \       'highlight_builtins' : 1
  \     },
  \     'cpp': {
  \       'highlight_standard_library': 1
  \     },
  \     'c': {
  \       'highlight_builtins' : 1
  \     }
  \   }
  \ }

" Color scheme

let g:airline_theme='yaflandia'
colorscheme yaflandia
```
