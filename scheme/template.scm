(use-modules (srfi srfi-1))

(define string-join-spaces
    (lambda (. strings)
        (string-join strings)))

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
     ((template-substring (rule-key rule))
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
          (string-append 
            string-before-template
            (rule-replacement rule)
            (substitute-rule
              rule
              string-after-template)))
         string))))

(define template-rule->macro-rule
  (lambda (rule)
    (make-rule 
      (string-append "${" (symbol->string (rule-key rule)) "}") 
      (rule-replacement rule))))

(define template-rule?
  (lambda (rule)
    (symbol? (rule-key rule))))

(define make-schema-node
  (lambda* (#:key rule prerequisites)
    (lambda (input-document)
      (let ((processed-document 
              (if rule
                  (substitute-rule 
                    (if (template-rule? rule) 
                        (template-rule->macro-rule rule)
                        rule)
                    input-document)
                  input-document)))
        (if prerequisites 
            (concatenate 
              (map 
                (lambda (edge) (edge processed-document))
                (if (list? prerequisites) 
                    prerequisites 
                    (list prerequisites))))
            (list processed-document))))))

(define make-chained-nodes
  (lambda* (#:key rules prerequisites)
           (fold 
             (lambda (rule chained-nodes)
               (make-schema-node #:rule rule #:prerequisites chained-nodes))
             prerequisites
             rules)))

(define generate-body-code
  (lambda (root-schema-node body-template)
    (let ((generated-documents ((root-schema-node) body-template))) 
     (string-join generated-documents "\n\n"))))

(define generate-code
  (lambda (schema-root-node headers body-template tail)
    (simple-format #t "~A" headers)
    (simple-format #t "~A" (generate-body-code schema-root-node body-template))
    (simple-format #t "~A" tail)))

