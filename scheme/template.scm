(define replace-template
    (lambda (template-key replacement string)
        (let* 
            ((template-substring 
                (string-join 
                    (list "${" template-key "}") 
                    ""))
            (template-start-index (string-contains string template-substring)))
            
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
                    (string-join 
                        (list 
                            string-before-template
                            replacement
                            (replace-template
                                template-key 
                                replacement 
                                string-after-template))
                        ""))
                string))))


(define replace-templates
    (lambda (key-replacements string)
        (if (null? key-replacements)
            string
            (let* 
                ((key-replacement (car key-replacements))
                (template-key (car key-replacement))
                (replacement (cdr key-replacement)))
                
                (replace-template 
                    template-key 
                    replacement 
                    (replace-templates (cdr key-replacements) string))))))

