(use-modules 
  (srfi srfi-1) 
  ((rnrs) :version (6)))

(define assure
  (lambda (predicate value)
    (assert (if (predicate value) value #f))))

(define string-join-spaces
    (lambda (. strings)
        (string-join strings)))

(define make-rule
  (lambda (key replacement)
    (cons (assure 
            (lambda (value) 
              (or (string? value) (symbol? value))) 
            key) 
          (assure string? replacement))))

(define rule-key
    (lambda (rule) (car rule)))

(define rule-replacement
    (lambda (rule) (cdr rule)))

(define substitute-rule
  (lambda (rule string)
    (let* 
     ((template-substring (rule-key rule))
      (template-start-index (string-contains (assure string? string) template-substring)))
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

(define make-dependency-node
  (lambda* (#:key rule dependent-nodes)
           (let* ((substitute-document
                    (lambda (rule document) 
                      (let ((rules 
                              (map 
                                (lambda (rule) 
                                  (if (template-rule? rule)
                                      (template-rule->macro-rule rule)
                                      rule))
                                (if (list? rule)
                                    rule
                                    (list rule))))) 
                        (fold substitute-rule document rules))))
                  (gather-processed-documents 
                    (lambda (output-document dependent-nodes)
                      (if dependent-nodes 
                          (concatenate 
                            (map 
                              (lambda (dependent-node) (dependent-node output-document))
                              (if (list? dependent-nodes) 
                                  dependent-nodes 
                                  (list dependent-nodes))))
                          (list output-document)))))
             (lambda (input-document) 
               (gather-processed-documents 
                 (if rule
                        (substitute-document rule input-document)
                        input-document)
                 dependent-nodes)))))

(define generate-text
  (lambda (schema template)
    (let ((generated-documents (((assure thunk? schema)) (assure string? template)))) 
     (string-join generated-documents "\n\n"))))

(define output-text
  (lambda (. text-blocks)
    (map 
      (lambda (text-block) (simple-format #t "~A" text-block))
      text-blocks)))

