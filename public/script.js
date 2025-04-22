// Récupérer les stations depuis l'API
// Script pour L'âme de Perpignan

document.addEventListener('DOMContentLoaded', function() {
    // Navigation lisse pour les ancres
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            e.preventDefault();
            
            const targetId = this.getAttribute('href');
            const targetElement = document.querySelector(targetId);
            
            if (targetElement) {
                window.scrollTo({
                    top: targetElement.offsetTop - 80, // Compensation pour le header fixe
                    behavior: 'smooth'
                });
            }
        });
    });
    
    // Gestion du formulaire d'itinéraire
    const routeForm = document.querySelector('.route-form');
    if (routeForm) {
        routeForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Récupération des valeurs des champs
            const depart = document.getElementById('depart').value;
            const arrivee = document.getElementById('arrivee').value;
            const intermediaire = document.getElementById('intermediaire').value;
            const eviter = document.getElementById('eviter').value;
            
            // Validation basique
            if (!depart || !arrivee) {
                alert('Veuillez saisir au moins un point de départ et d\'arrivée.');
                return;
            }
            
            // Simulation de recherche d'itinéraire
            console.log('Recherche d\'itinéraire:');
            console.log('- Départ:', depart);
            console.log('- Arrivée:', arrivee);
            
            if (intermediaire) {
                console.log('- Points intermédiaires:', intermediaire);
            }
            
            if (eviter) {
                console.log('- Points à éviter:', eviter);
            }
            
            // Dans une vraie application, vous feriez un appel API ici
            alert(`Itinéraire calculé de ${depart} à ${arrivee} ! (Simulation)`);
        });
    }
    
    // Animation des cartes au survol (optionnel)
    const cards = document.querySelectorAll('.category-card, .discover-card');
    
    cards.forEach(card => {
        card.addEventListener('mouseenter', function() {
            this.style.transform = 'translateY(-10px)';
            this.style.boxShadow = '0 15px 25px rgba(0, 0, 0, 0.2)';
        });
        
        card.addEventListener('mouseleave', function() {
            this.style.transform = 'translateY(0)';
            this.style.boxShadow = '0 5px 15px rgba(0, 0, 0, 0.1)';
        });
    });
    
    // Menu hamburger pour mobile (à implémenter si nécessaire)
    // Cette partie serait à développer si vous souhaitez un menu hamburger sur mobile
    
    // Animation au défilement (optionnel)
    function checkScroll() {
        const elements = document.querySelectorAll('.category-card, .feature, .discover-card');
        const windowHeight = window.innerHeight;
        
        elements.forEach(element => {
            const elementPosition = element.getBoundingClientRect().top;
            
            if (elementPosition < windowHeight - 100) {
                element.classList.add('visible');
            }
        });
    }
    
    // Initialisation
    window.addEventListener('scroll', checkScroll);
    checkScroll(); // Vérification initiale
  });