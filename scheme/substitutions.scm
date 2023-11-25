(load-from-path "template.scm")


(define vector-rule
  '(vector-type . "Vector${struct-type-suffix}"))

(define double-schema
  (lambda* (#:optional edges) 
           (make-chained-nodes 
             #:rules '((number-base-type . "double") 
                       (struct-type-precision . "Double")) 
             #:prerequisites edges)))

(define float-schema
  (lambda* (#:optional edges) 
           (make-chained-nodes 
             #:rules '((number-base-type . "float") 
                       (struct-type-precision . "Float")) 
             #:prerequisites edges)))

(define number-schema
  (lambda* (#:optional edges) 
           (make-schema-node 
             #:prerequisites (list 
                               (double-schema edges)
                               (float-schema edges)))))

(define complex-number-schema
  (lambda* (#:optional edges) 
           (make-chained-nodes 
             #:rules '((number-type . "${number-base-type} complex") 
                       (struct-type-suffix . "Complex${struct-type-precision}")
                       (function-tag . "_complex_${number-base-type}"))
             #:prerequisites (number-schema edges))))

(define real-number-schema
  (lambda* (#:optional edges) 
           (make-chained-nodes 
             #:rules '((number-type . "${number-base-type}") 
                       (struct-type-suffix . "Real${struct-type-precision}")
                       (function-tag . "_real_${number-base-type}"))
             #:prerequisites (number-schema edges))))

(define numeric-schema
  (lambda* (#:optional edges)
           (make-schema-node
             #:prerequisites (list 
                               (complex-number-schema edges) 
                               (real-number-schema edges)))))

(define vector-schema
  (lambda* (#:optional edges)
           (make-schema-node 
             #:rule vector-rule 
             #:prerequisites (numeric-schema edges))))
