 ## Normal mode usefull shortcuts

*normal:*

*`*: mode-enter set_mark \
*'*: mode-enter jump_mark

*+*: zoom-in \
*-*: zoom-out \
*=*: zoom 100%

*.*: repeat-command

### Hints
*;I*: hint images tab \
*;O*: hint links fill :open -t -r {hint-url} | *;o*: hint links fill :open {hint-url} \
*;R*: hint --rapid links window | *;r*: hint --rapid links tab-bg \
*;Y*: hint links yank-primary | *;y*: hint links yank \
*;b*: hint all tab-bg \
*;d*: hint links download \
*;f*: hint all tab-fg \
*;h*: hint all hover \
*;i*: hint images \
*;t*: hint inputs

*B*: set-cmd-text -s :quickmark-load -t | *b*: set-cmd-text -s :quickmark-load \
*F*: hint all tab | *f*: hint

*gi*: hint inputs --first

*H*: back \
*J*: tab-next \
*K*: tab-prev \
*L*: forward \
*M*: bookmark-add \
*N*: search-prev \
*O*: set-cmd-text -s :open -t \
*R*: reload -f | r: reload \
*U*: undo -w \
*V*: mode-enter caret ;; selection-toggle --line

### Scrolling
*G*: scroll-to-perc \
*gg*: scroll-to-perc 0

### Search page for bookmarks/history
*Sb*: bookmark-list --jump \
*Sh*: history \
*Sq*: bookmark-list \
*Ss*: set

### Tabs
*T*: tab-focus \
*D*: tab-close -o | d: tab-close \
*co*: tab-only (close other tabs) \
*g$*: tab-focus -1 \
*g0*: tab-focus 1 \

*gB*: set-cmd-text -s :bookmark-load -t | *gb*: set-cmd-text -s :bookmark-load \
*gO*: set-cmd-text :open -t -r {url:pretty} | *go*: set-cmd-text :open {url:pretty}

*gC*: tab-clone \
*gD*: tab-give \
*gt*: set-cmd-text -s :tab-select

*gm*: tab-move \
*gJ*: tab-move + \
*gK*: tab-move -

*gU*: navigate up -t \
*g^*: tab-focus 1 \
*ga*: open -t \
*gu*: navigate up

*h*: scroll left \
*i*: mode-enter insert \
*j*: scroll down \
*k*: scroll up \
*l*: scroll right \
*m*: quickmark-save \
*n*: search-next \
*o*: set-cmd-text -s :open \
*u*: undo \
*v*: mode-enter caret

*th*: back -t \
*tl*: forward -t


### New window
*wB*: set-cmd-text -s :bookmark-load -w \
*wIf*: devtools-focus \
*wIh*: devtools left \
*wIj*: devtools bottom \
*wIk*: devtools top \
*wIl*: devtools right \
*wIw*: devtools window \
*wO*: set-cmd-text :open -w {url:pretty} \
*wP*: open -w -- {primary} \
*wb*: set-cmd-text -s :quickmark-load -w \
*wf*: hint all window \
*wh*: back -w \
*wi*: devtools \
*wl*: forward -w \
*wo*: set-cmd-text -s :open -w \
*wp*: open -w -- {clipboard}

*xO*: set-cmd-text :open -b -r {url:pretty} \
*xo*: set-cmd-text -s :open -b

### Clipboards
*yD*: yank domain -s | *yd*: yank domain \
*yP*: yank pretty-url -s | *yp*: yank pretty-url \
*yT*: yank title -s | *yt*: yank title \
*yY*: yank -s | *yy*: yank

*PP*: open -t -- {primary} | *pP*: open -- {primary} \
*Pp*: open -t -- {clipboard} | *pp*: open -- {clipboard}

### Macro
*q*: macro-record \
*@*: macro-play

### Various
*gd*: download \
*ad*: download-cancel \
*cd*: download-clear

*gf*: view-source
