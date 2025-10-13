/* Add navigation bar to return to main documentation */
(function() {
    // Wait for DOM to be ready
    function addBackNavigation() {
        console.log('Adding back navigation...');
        
        // Calculate relative path to main documentation
        var currentPath = window.location.pathname;
        var backPath = '../../index.html'; // Go up two levels: html/ -> developer-guide/ -> root/
        
        console.log('Current path:', currentPath);
        console.log('Back path:', backPath);
        
        // Create navigation bar HTML
        var navBar = document.createElement('div');
        navBar.id = 'back-to-main-nav';
        navBar.style.cssText = 'position: fixed; top: 0; left: 0; right: 0; background-color: #007dc3; color: white; padding: 10px 20px; z-index: 9999; box-shadow: 0 2px 4px rgba(0,0,0,0.1); font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;';
        
        var link = document.createElement('a');
        link.href = backPath;
        link.innerHTML = '🏠 Back to Main Documentation';
        link.style.cssText = 'color: white; text-decoration: none; font-weight: bold; display: inline-flex; align-items: center; gap: 8px;';
        
        navBar.appendChild(link);
        
        // Insert at the beginning of body
        document.body.insertBefore(navBar, document.body.firstChild);
        
        // Add margin to body to account for fixed nav
        document.body.style.marginTop = '50px';
        
        console.log('Navigation bar added successfully');
    }
    
    // Multiple ways to ensure the script runs
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', addBackNavigation);
    } else if (document.readyState === 'interactive' || document.readyState === 'complete') {
        addBackNavigation();
    }
    
    // Fallback with jQuery if available
    if (typeof $ !== 'undefined') {
        $(document).ready(addBackNavigation);
    }
    
    // Additional fallback with timeout
    setTimeout(addBackNavigation, 100);
})();
