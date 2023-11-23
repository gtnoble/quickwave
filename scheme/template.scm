(define string-concatentate
    (lambda (. strings)
        (string-join strings "")))

(define make-rule
    (lambda (key replacement)
        (cons key replacement)))

(define rule-key
    (lambda (rule) (car rule)))

(define rule-replacement
    (lambda (rule) (cdr rule)))

(define substitute-rule
    (lambda (rule string)
        (let* 
            ((template-substring 
                (string-concatentate
                    "${" (symbol->string (rule-key rule)) "}"))
            (template-start-index (string-contains-ci string template-substring)))
            
            (if template-start-index 
                (let 
                    ((string-before-template 
                        (string-take string template-start-index))
                    (string-after-template 
                        (string-drop
                            string
                            (+ 
                                template-start-index 
                                (string-length template-substring)))))
                    (string-concatentate 
                        string-before-template
                        (rule-replacement rule)
                        (substitute-rule
                            (rule-key rule)
                            (rule-replacement rule) 
                            string-after-template)))
                string))))


(define substitute-rules
    (lambda (rules string)
        (if (null? rules)
            string
            (substitute-rule 
                (car rules)
                (substitute-rules (cdr rules) string)))))

(define make-substitution
    (lambda (type-tag rules)
        (cons type-tag rules)))

(define substitution-tag
    (lambda (substitution)
        (car substitution)))

(define substitution-rules
    (lambda (substitution)
        (cdr substitution)))

(define combine-substitutions
    (lambda (current-substitutions substitutions-to-combine)
        (if (null? substitutions-to-combine) 
            `()
            (let* 
                ((substitution-to-combine (car substitutions-to-combine))
                (combined-tag (substitution-tag substitution-to-combine))
                (rules-to-combine (substitution-rules substitution-to-combine))
                (current-rules 
                    (substitution-rules 
                        (assq-ref 
                            current-substitutions 
                            combined-tag))))

                (cons 
                    (make-substitution 
                        combined-tag
                        (if current-rules
                            (combine-rules
                                (combine-rules 
                                    rules-to-combine
                                    current-rules)
                                (combine-rules 
                                    current-rules 
                                    rules-to-combine))
                            rules-to-combine))
                    (combine-substitutions 
                        current-substitutions 
                        (cdr substitutions-to-combine)))))))


(define combine-rules
    (lambda (current-rules rules-to-add)
        (if (null? rules-to-add)
            current-rules
            (let* 
                ((next-rule (car rules-to-add))
                (already-have-rule 
                    (assq 
                        (rule-key next-rule)
                        current-rules)))

                (combine-rules
                    (if already-have-rule
                        current-rules
                        (cons next-rule current-rules))
                    (cdr rules-to-add))))))


(define generate-body-code
    (lambda (substitutions body-template)
        (if (null? substitutions)
            "\n\n"
            (string-concatentate 
                (substitute-rules 
                    (substitution-rules 
                        (car substitutions)) 
                        body-template) 
                "\n\n" 
                (generate-body-code (cdr substitutions) body-template)))))

(define generate-code
    (lambda (substitutions headers body-template tail)
        (simple-format #t "~A" headers)
        (simple-format #t "~A" (generate-body-code substitutions body-template))
        (simple-format #t "~A" tail)))
