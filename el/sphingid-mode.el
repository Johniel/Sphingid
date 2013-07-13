(require 'generic)

(define-generic-mode sphingid-mode
  ;; comment
  '("//")
  ;; keyword
  '("class" "struct"
    "true" "false"
    "if" "for" "while" "do"
    "return" "continue" "break")
  ;; regexp and face
  '(("int\\|char\\|string\\|double\\|lli" . 'font-lock-type-face))
  ;;
  '("\\.sp\\'")
  ;; hooks
  '()
  ;;
  "Major mode for Sphingid")

(provide 'sphingid-mode)
