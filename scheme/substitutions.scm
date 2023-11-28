(load-from-path "template.scm")

(define make-precursor-node
  (lambda* (nodes precursors #:optional rules)
           (let ((nodes-dependencies
                   (map 
                     (lambda (make-precursor-node)
                       (make-precursor-node nodes))
                     precursors))) 
             (make-schema-node 
               #:rule rules 
               #:dependent-nodes nodes-dependencies))))


(define make-precursor
  (lambda* (#:key rules precursors)
           (lambda* (#:optional nodes)
                    (if precursors
                        (make-precursor-node nodes precursors rules)
                        (make-schema-node #:rule rules #:dependent-nodes nodes)))))

(define double-precursor
  (make-precursor
    #:rules '((number-base-type . "double") 
              (math-function-suffix . "")
              (function-tag . "_double")
              (struct-type-suffix . "Double"))))

(define float-precursor
  (make-precursor
    #:rules '((number-base-type . "float") 
              (math-function-suffix . "f")
              (function-tag . "_float")
              (struct-type-suffix . "Float"))))

(define number-precursor
  (make-precursor
    #:precursors (list double-precursor float-precursor)))

(define number-schema
  (number-precursor))

(define complex-number-precursor
  (make-precursor
    #:rules '((number-type . "${number-base-type} complex") 
              (struct-type-suffix . "Complex${struct-type-suffix}")
              (function-tag . "_complex_${number-base-type}"))
    #:precursors (list number-precursor)))

(define real-number-precursor
  (make-precursor
    #:rules '((number-type . "${number-base-type}") 
              (struct-type-suffix . "Real${struct-type-suffix}")
              (function-tag . "_real_${number-base-type}"))
    #:precursors (list number-precursor)))

(define real-number-schema
  (real-number-precursor))

(define numeric-precursor
  (make-precursor
    #:precursors (list complex-number-precursor  
                          real-number-precursor)))

(define vector-precursor
  (make-precursor
    #:rules '((vector-type . "Vector${struct-type-suffix}"))
    #:precursors (list numeric-precursor)))

(define vector-schema
  (vector-precursor))

(define filter-precursor
  (make-precursor
    #:rules '((filter-type . "DigitalFilter${struct-type-suffix}"))
    #:precursors (list vector-precursor)))

(define filter-schema
  (filter-precursor))

(define moving-average-precursor
  (make-precursor
    #:rules '((moving-average-type . "MovingAverage${struct-type-suffix}"))
    #:precursors (list vector-precursor)))

(define moving-average-schema
  (moving-average-precursor))

(define oscillator-precursor
  (make-precursor
    #:rules '((oscillator-type . "Oscillator${struct-type-suffix}"))
    #:precursors (list number-precursor)))

(define oscillator-schema
  (oscillator-precursor))
